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

    PONY_GUARD_BY(FRAME) virtual void setTrack(int i) {
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
private:
    struct {
        qreal videoDuration = std::numeric_limits<qreal>::quiet_NaN();
        qreal audioDuration = std::numeric_limits<qreal>::quiet_NaN();
        std::vector<StreamIndex> m_videoStreamsIndex;
        std::vector<StreamIndex> m_audioStreamsIndex;
        std::vector<StreamInfo> streamInfos;
    } description;
    LifeCycleManager *m_lifeManager = new LifeCycleManager;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
    StreamIndex m_audioStreamIndex;
    StreamIndex m_videoStreamIndex;
    IDemuxDecoder *audioDecoder;
    IDemuxDecoder *videoDecoder;

    std::atomic<bool> interrupt = true;
    AVPacket *packet = nullptr;

public:
    explicit DecodeDispatcher(
      const std::string &fn,
      StreamIndex audioStreamIndex = DEFAULT_STREAM_INDEX,
      StreamIndex videoStreamIndex = DEFAULT_STREAM_INDEX,
      QObject *parent = nullptr
    ) : DemuxDispatcherBase(fn, parent), m_audioStreamIndex(audioStreamIndex), m_videoStreamIndex(videoStreamIndex) {
        packet = av_packet_alloc();
        for (StreamIndex i = 0; i < fmtCtx->nb_streams; ++i) {
            auto *stream = fmtCtx->streams[i];
            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                description.m_videoStreamsIndex.emplace_back(i);
            } else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                description.m_audioStreamsIndex.emplace_back(i);
            }
            StreamInfo info(stream);
            description.streamInfos.emplace_back(stream);
        }

        // audio
        if (description.m_audioStreamsIndex.empty()) { throw std::runtime_error("Cannot find audio stream."); }
        if (m_audioStreamIndex == DEFAULT_STREAM_INDEX) { m_audioStreamIndex = description.m_audioStreamsIndex.front(); }
        audioQueue = new TwinsBlockQueue<AVFrame *>("AudioQueue", 16);
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_lifeManager);
        description.audioDuration = audioDecoder->duration();

        // video
        videoQueue = audioQueue->twins("VideoQueue", 16);
        if (description.m_videoStreamsIndex.empty()) {
            // no audio
            videoDecoder = new VirtualVideoDecoder(description.audioDuration);
        } else {
            if (m_videoStreamIndex == DEFAULT_STREAM_INDEX) { m_videoStreamIndex = description.m_videoStreamsIndex.front(); }
            videoDecoder = new DecoderImpl<Video>(fmtCtx->streams[m_videoStreamIndex], videoQueue, m_lifeManager);
        }
        description.videoDuration = videoDecoder->duration();

        connect(this, &DecodeDispatcher::signalStartWorker, this, &DecodeDispatcher::onWork, Qt::QueuedConnection);
    }

    ~DecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        DecodeDispatcher::flush();
        delete audioQueue;
        delete videoQueue;
        delete audioDecoder;
        delete videoDecoder;
        if(packet) { av_packet_free(&packet); }
        m_lifeManager->deleteLater();
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
        if (interrupt.exchange(false)) {
            videoQueue->open();
            emit signalStartWorker(QPrivateSignal());
            qDebug() << "Queue stateResume.";
        }
        // ignore when already start

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
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(secs * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (audioDecoder) { audioDecoder->flushFFmpegBuffers(); }
        if (videoDecoder) { videoDecoder->flushFFmpegBuffers(); }
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    PONY_THREAD_SAFE VideoFrame getPicture (bool b, bool own) override { return videoDecoder->getPicture(b, own); }

    PONY_THREAD_SAFE bool popPicture(bool b) override { return videoDecoder->pop(b); }

    PONY_THREAD_SAFE AudioFrame getSample(bool b) override { return audioDecoder->getSample(b); }

    PONY_THREAD_SAFE bool popSample(bool b) override { return audioDecoder->pop(b); }

    PONY_GUARD_BY(MAIN, FRAME, DECODER) [[nodiscard]] qreal getAudionLength() { return description.audioDuration; }
    PONY_GUARD_BY(MAIN, FRAME, DECODER) [[nodiscard]] qreal getVideoLength() { return description.videoDuration; }

    PONY_GUARD_BY(FRAME) void setTrack(int i) override {
        delete audioDecoder;
        m_audioStreamIndex = description.m_audioStreamsIndex[static_cast<size_t>(i)];
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_lifeManager);
    }


public slots:
    void onWork() override {
        videoQueue->open();
        while(!interrupt) {
            AVFrame *frame;
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
        interrupt = true;
    };

    void setAudioIndex(StreamIndex i) override {
        if (i == m_audioStreamIndex) { return; }
        delete audioDecoder;
        m_audioStreamIndex = i;
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_lifeManager);
    }

    QStringList getTracks() {
        QStringList ret;
        ret.reserve(static_cast<qsizetype>(description.m_audioStreamsIndex.size()));
        for(auto && i : description.m_audioStreamsIndex) {
            ret.emplace_back(description.streamInfos[i].getFriendName());
        }
        return ret;
    }
signals:
    void signalStartWorker(QPrivateSignal);
};

/**
 * @brief 反向解码器调度器
 */

class ReverseDecodeDispatcher : public DemuxDispatcherBase {
    Q_OBJECT
    PONY_THREAD_AFFINITY(DECODER)
private:
    int videoStreamIndex{-1};

    AVStream *videoStream{};

    ReverseDecoderImpl<Video> *videoDecoder;

    AVFrame *dummy;
    AudioFrame silenceFrame;
    std::vector<StreamInfo> streamInfos;
    std::byte silence[1024]{};

    std::atomic<bool> interrupt = true;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<std::vector<AVFrame *>*> *videoQueue;
    TwinsBlockQueue<std::vector<AVFrame *>*> *audioQueue;
    qreal m_videoDuration;
public:
    explicit ReverseDecodeDispatcher(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent),
    dummy(av_frame_alloc()),
    silenceFrame(silence, 1024, std::numeric_limits<double>::max(), dummy){
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
        m_videoDuration = videoDecoder->duration();
        connect(this, &ReverseDecodeDispatcher::signalStartWorker, this, &ReverseDecodeDispatcher::onWork, Qt::QueuedConnection);
    }

    ~ReverseDecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        if (videoQueue) { videoQueue->close(); }
        if (audioQueue) { audioQueue->close(); }
        delete videoDecoder;
        if(packet) { av_packet_free(&packet); }
        if (dummy) { av_frame_free(&dummy);}
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
                if (frame) av_frame_free(&frame);
        });
        audioQueue->clear([](std::vector<AVFrame *>* frameStk){
            for (auto frame : *frameStk)
                if (frame) av_frame_free(&frame);
        });
    }


    /**
     * 保证可以阻塞地获取 Picture 和 Sample. 这个方法是线程安全的.
     */
    PONY_THREAD_SAFE void stateResume() override {
        interrupt = false;
        videoQueue->open();
        emit signalStartWorker(QPrivateSignal());
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

public slots:
    void onWork() override {
        videoQueue->open();
        while(!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (packet->stream_index == videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                    // qDebug() << "accept";
                    auto next = videoDecoder->nextSegment();
                    if (next > 0) {
                        videoDecoder->flushFFmpegBuffers();
                        av_seek_frame(fmtCtx, videoStreamIndex,
                                      static_cast<int64_t>((next-1.0) / av_q2d(videoStream->time_base)),
                                      AVSEEK_FLAG_BACKWARD);
                    }
                    else if (next == 0) {
                        av_packet_unref(packet);
                        videoDecoder->pushEOF();
                        qDebug() << "reverse: finish";
                        break;
                    }
                }
            } else if (ret == ERROR_EOF) {
                //std::cerr << "reverse reach eof" << std::endl;
                qDebug() << "reverse: reach eof";
                videoDecoder->flushFFmpegBuffers();
                av_seek_frame(fmtCtx, videoStreamIndex,
                              static_cast<int64_t>((m_videoDuration-2.0) / av_q2d(videoStream->time_base)),
                              AVSEEK_FLAG_BACKWARD);
                videoDecoder->setStart(m_videoDuration-1.0);
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            av_packet_unref(packet);
        }
        interrupt = true;
    };

signals:
    void signalStartWorker(QPrivateSignal);
};



