//
// Created by ColorsWind on 2022/4/30.
// Adapted from demuxer v1 by kurisu on 2022/4/16.
//
#pragma once

#include <QtCore>
#include <QTimer>
#include <unordered_map>
#include <vector>
#include "ponyplayer.h"
#include "helper.hpp"
#include "frame.hpp"
#include "twins_queue.hpp"
#include "decoder.hpp"

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

//#define IGNORE_VIDEO_FRAME


class StreamInfo {
private:
    int index;
    std::unordered_map<std::string, std::string> dict;
    qreal duration;
public:
    StreamInfo(AVStream *stream) : index(stream->index) {
        AVDictionaryEntry *tag = nullptr;
        while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            dict[tag->key] = tag->value;
        }
        duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    [[nodiscard]] qreal getDuration() const { return duration; }

    [[nodiscard]] QString getFriendName() const {
        QString str;
        if (auto iter=dict.find("language"); iter !=dict.cend()) {
            str.append(iter->second.c_str());
            str.append(" | ");
        }
        auto minutes = static_cast<int>(duration) / 60;
        auto seconds = duration - minutes * 60;
        if (minutes > 0) {str.append(QString::number(minutes)).append("m"); }
        str.append(QString::number(seconds, 'f', 1).append("s"));
        return str;
    }

};

typedef unsigned int StreamIndex;
const constexpr StreamIndex DEFAULT_STREAM_INDEX = std::numeric_limits<StreamIndex>::max();

class DemuxDispatcherBase: public QObject {
    Q_OBJECT
public:
    const std::string filename;
protected:

    AVFormatContext *fmtCtx = nullptr;

    explicit DemuxDispatcherBase(const std::string &fn, QObject *parent) : QObject(parent), filename(fn) {
        if (avformat_open_input(&fmtCtx, fn.c_str(), nullptr, nullptr) < 0) {
            throw std::runtime_error("Cannot open input file.");
        }
        if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
            throw std::runtime_error("Cannot find any stream in file.");
        }
    }

    ~DemuxDispatcherBase() override {
        if (fmtCtx) { avformat_close_input(&fmtCtx); }
    }
public:
    PONY_GUARD_BY(FRAME) virtual void statePause() {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual void flush() {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual void stateResume() {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual void seek(qreal secs) {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual VideoFrame getPicture(bool b, bool own) {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual bool popPicture(bool b) {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual AudioFrame getSample(bool b) {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_GUARD_BY(FRAME) virtual bool popSample(bool b) {

        throw std::runtime_error("Unsupported operation.");
    }

    PONY_THREAD_SAFE virtual qreal getAudionLength() {
        throw std::runtime_error("Unsupported operation.");
    }
    PONY_THREAD_SAFE virtual qreal getVideoLength() {
        throw std::runtime_error("Unsupported operation.");
    }

    PONY_THREAD_SAFE [[nodiscard]] virtual const std::vector<StreamIndex>& audioIndex() const {
        throw std::runtime_error("Unsupported operation.");
    }
    PONY_THREAD_SAFE [[nodiscard]] virtual const std::vector<StreamIndex>& videoIndex() const {
        throw std::runtime_error("Unsupported operation.");
    }
    PONY_THREAD_SAFE [[nodiscard]] virtual const std::vector<StreamInfo>& streamsInfo() const {
        throw std::runtime_error("Unsupported operation.");
    }
    PONY_THREAD_SAFE [[nodiscard]] virtual const StreamInfo& getStreamInfo(StreamIndex i)  const {
        throw std::runtime_error("Unsupported operation.");
    }

public slots:
    virtual void onWork() {
        throw std::runtime_error("Unsupported operation.");
    }
    virtual void setAudioIndex(StreamIndex i) {
        throw std::runtime_error("Unsupported operation.");
    }
};

/**
 * @brief 解码器调度器, 将Packet分配给解码器进一步解码成Frame
 * 这个类是RAII的
 */
class DecodeDispatcher : public DemuxDispatcherBase {
    Q_OBJECT
    std::vector<StreamIndex> m_videoStreamsIndex;
    std::vector<StreamIndex> m_audioStreamsIndex;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
    StreamIndex m_audioStreamIndex;
    StreamIndex m_videoStreamIndex;
    IDemuxDecoder *audioDecoder;
    IDemuxDecoder *videoDecoder;

    std::vector<StreamInfo> streamInfos;
    std::atomic<bool> interrupt = false;
    AVPacket *packet = nullptr;
    FrameFreeFunc *m_freeFunc;
    FrameFreeQueue *m_freeQueue;

public:
    explicit DecodeDispatcher(
      const std::string &fn,
      FrameFreeQueue *freeQueue,
      FrameFreeFunc *freeFunc,
      StreamIndex audioStreamIndex = DEFAULT_STREAM_INDEX,
      StreamIndex videoStreamIndex = DEFAULT_STREAM_INDEX,
      QObject *parent = nullptr
    ) : DemuxDispatcherBase(fn, parent), m_audioStreamIndex(audioStreamIndex), m_videoStreamIndex(videoStreamIndex),
        m_freeFunc(freeFunc), m_freeQueue(freeQueue){
        packet = av_packet_alloc();
        for (StreamIndex i = 0; i < fmtCtx->nb_streams; ++i) {
            auto *stream = fmtCtx->streams[i];
            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                m_videoStreamsIndex.emplace_back(i);
            } else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                m_audioStreamsIndex.emplace_back(i);
            }
            StreamInfo info(stream);
            streamInfos.emplace_back(stream);
        }
        if (m_videoStreamsIndex.empty()) { throw std::runtime_error("Cannot find video stream."); }
        if (m_audioStreamsIndex.empty()) { throw std::runtime_error("Cannot find audio stream."); }
        if (m_videoStreamIndex == DEFAULT_STREAM_INDEX) { m_videoStreamIndex = m_videoStreamsIndex.front(); }
        if (m_audioStreamIndex == DEFAULT_STREAM_INDEX) { m_audioStreamIndex = m_audioStreamsIndex.front(); }


        // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
        videoQueue = new TwinsBlockQueue<AVFrame *>("VideoQueue", 16);
        audioQueue = videoQueue->twins("AudioQueue", 16);
        videoDecoder = new DecoderImpl<Video>(fmtCtx->streams[m_videoStreamIndex], videoQueue, freeQueue, freeFunc);
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, freeQueue, freeFunc);

        interrupt = false; // happens before
    }

    ~DecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        AVFrame *frame;
        while(m_freeQueue->try_dequeue(frame)) { av_frame_free(&frame); }
        DecodeDispatcher::flush();
        delete audioQueue;
        delete videoQueue;
        delete audioDecoder;
        delete videoDecoder;
        if(packet) { av_packet_free(&packet); }

    }

    /**
     * 保证阻塞获取结果的线程尽快被唤醒, 同时请求 DecodeThread 的工作尽快停止.
     */
    void statePause() override {
        interrupt = true;
        videoQueue->close();
        qDebug() << "Queue close.";
    }

    void flush() override {
        videoQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
        audioQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
    }


    /**
     * 保证可以阻塞地获取 Picture 和 Sample. 这个方法是线程安全的.
     */
    void stateResume() override {
        interrupt = false;
        videoQueue->open();
        qDebug() << "Queue stateResume.";
    }

    /**
     * 修改视频播放进度, 注意: 这个方法必须在解码线程上调用.
     * @param secs 新的视频进度(单位: 秒)
     */
    void seek(qreal secs) override {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        qDebug() << "a Seek:" << secs;
        AVFrame *frame;
        while(m_freeQueue->try_dequeue(frame)) { av_frame_free(&frame); }
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(secs * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (audioDecoder) { audioDecoder->flushFFmpegBuffers(); }
        if (videoDecoder) { videoDecoder->flushFFmpegBuffers(); }
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    VideoFrame getPicture (bool b, bool own) override { return videoDecoder->getPicture(b, own); }

    bool popPicture(bool b) override { return videoDecoder->pop(b); }

    AudioFrame getSample(bool b) override { return audioDecoder->getSample(b); }

    bool popSample(bool b) override { return audioDecoder->pop(b); }

    [[nodiscard]] qreal getAudionLength() override { return audioDecoder->duration(); }
    [[nodiscard]] qreal getVideoLength() override { return videoDecoder->duration(); }

    [[nodiscard]] const std::vector<StreamIndex>& audioIndex() const override { return m_audioStreamsIndex; }
    [[nodiscard]] const std::vector<StreamIndex>& videoIndex() const override { return m_videoStreamsIndex; }
    [[nodiscard]] const std::vector<StreamInfo>& streamsInfo() const override { return streamInfos; }
    [[nodiscard]] const StreamInfo& getStreamInfo(StreamIndex i) const override { return streamInfos[i]; }
public slots:
    void onWork() override {
        videoQueue->open();
        while(!interrupt) {
            AVFrame *frame;
            while(m_freeQueue->try_dequeue(frame)) { av_frame_free(&frame); }
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (static_cast<StreamIndex>(packet->stream_index) == m_videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                } else if (static_cast<StreamIndex>(packet->stream_index) == m_audioStreamIndex) {
                    audioDecoder->accept(packet, interrupt);
                }
            } else if (ret == ERROR_EOF) {
                videoQueue->push(nullptr);
                audioQueue->push(nullptr);
                av_packet_unref(packet); break;
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            av_packet_unref(packet);
        }
    };

    void setAudioIndex(StreamIndex i) override {
        if (i == m_audioStreamIndex) { return; }
        delete audioDecoder;
        m_audioStreamIndex = i;
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_freeQueue, m_freeFunc);
    }
};

/**
 * @brief 反向解码器调度器
 */
//#include <iostream>

class ReverseDecodeDispatcher : public DemuxDispatcherBase {
    Q_OBJECT
    PONY_THREAD_AFFINITY(DECODER)
private:
    int videoStreamIndex{-1};
    std::vector<StreamIndex> m_videoStreamsIndex;
    std::vector<StreamIndex> m_audioStreamsIndex;

    AVStream *videoStream{};

    ReverseDecoderImpl<Video> *videoDecoder;

    AudioFrame silenceFrame;
    std::vector<StreamInfo> streamInfos;
    std::byte silence[1024]{};

    std::atomic<bool> interrupt = false;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<std::vector<AVFrame *>*> *videoQueue;
    TwinsBlockQueue<std::vector<AVFrame *>*> *audioQueue;
public:
    explicit ReverseDecodeDispatcher(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent),
                                                                               silenceFrame(silence, 1024, std::numeric_limits<double>::max(), nullptr){
        packet = av_packet_alloc();
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
                videoStreamIndex = i;
                videoStream = fmtCtx->streams[i];
            }
        }

        if (videoStreamIndex < 0) { throw std::runtime_error("Cannot find video stream."); }

        // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
        videoQueue = new TwinsBlockQueue<std::vector<AVFrame *>*>("VideoQueue", 8);
        audioQueue = videoQueue->twins("AudioQueue", 8);
        videoDecoder = new ReverseDecoderImpl<Video>(fmtCtx->streams[videoStreamIndex], videoQueue);

        interrupt = false; // happens before
    }

    ~ReverseDecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        if (videoQueue) { videoQueue->close(); }
        if (audioQueue) { audioQueue->close(); }
        delete videoDecoder;
        if(packet) { av_packet_free(&packet); }
    }

    /**
     * 保证阻塞获取结果的线程尽快被唤醒, 同时请求 DecodeThread 的工作尽快停止.
     */
    PONY_THREAD_SAFE void statePause() override {
        interrupt = true;
        videoQueue->close();
        qDebug() << "Queue close.";
    }

    PONY_THREAD_SAFE void flush() override {
        videoQueue->clear([](std::vector<AVFrame *>* frameStk){
            for (auto frame : *frameStk)
                av_frame_free(&frame);
        });
        audioQueue->clear([](std::vector<AVFrame *>* frameStk){
            for (auto frame : *frameStk)
                av_frame_free(&frame);
        });
    }


    /**
     * 保证可以阻塞地获取 Picture 和 Sample. 这个方法是线程安全的.
     */
    PONY_THREAD_SAFE void stateResume() override {
        interrupt = false;
        videoQueue->open();
        qDebug() << "Queue stateResume.";
    }

    /**
     * 修改视频播放进度, 注意: 这个方法必须在解码线程上调用.
     * @param secs 新的视频进度(单位: 秒)
     */
    PONY_GUARD_BY(DECODER) void seek(qreal secs) override {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        videoDecoder->flushFFmpegBuffers();
        qDebug() << "a Seek:" << secs;
        secs = fmax(secs, 0.0);
        videoDecoder->setStart(secs);
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>((secs-1.0) * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    PONY_GUARD_BY(FRAME) VideoFrame getPicture(bool b, bool own) override { return videoDecoder->getPicture(b, own); }

    PONY_GUARD_BY(FRAME) bool popPicture(bool b) override { return videoDecoder->pop(b); }

    PONY_GUARD_BY(FRAME) AudioFrame getSample(bool b) override { return silenceFrame; }

    PONY_GUARD_BY(FRAME) bool popSample(bool b) override { return true; }

    PONY_GUARD_BY(MAIN) qreal getAudionLength() override { return {}; }
    PONY_THREAD_SAFE qreal getVideoLength() override { return videoDecoder->duration(); }

    [[nodiscard]] const std::vector<StreamIndex>& audioIndex() const override { return m_audioStreamsIndex; }
    [[nodiscard]] const std::vector<StreamIndex>& videoIndex() const override { return m_videoStreamsIndex; }
    [[nodiscard]] const std::vector<StreamInfo>& streamsInfo() const override { return streamInfos; }
    [[nodiscard]] const StreamInfo& getStreamInfo(StreamIndex i) const override { return streamInfos[i]; }

public slots:
    void onWork() override {
        videoQueue->open();
        while(!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (packet->stream_index == videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                    auto next = videoDecoder->nextSegment();
                    if (next > 0) {
                        videoDecoder->flushFFmpegBuffers();
                        av_seek_frame(fmtCtx, videoStreamIndex,
                                      static_cast<int64_t>((next-1.0) / av_q2d(videoStream->time_base)),
                                      AVSEEK_FLAG_BACKWARD);
                    }
                    else if (next == 0) {
                        av_packet_unref(packet);
                        //std::cerr << "reverse finish" << std::endl;
                        qDebug() << "reverse: finish";
                        break;
                    }
                }
            } else if (ret == ERROR_EOF) {
                //std::cerr << "reverse reach eof" << std::endl;
                qDebug() << "reverse: reach eof";
                videoDecoder->flushFFmpegBuffers();
                av_seek_frame(fmtCtx, videoStreamIndex,
                              static_cast<int64_t>((getVideoLength()-2.0) / av_q2d(videoStream->time_base)),
                              AVSEEK_FLAG_BACKWARD);
                videoDecoder->setStart(getVideoLength()-1.0);
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            av_packet_unref(packet);
        }
    };
};



