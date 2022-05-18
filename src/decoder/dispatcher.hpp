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
#include "forward.hpp"
#include "backward.hpp"

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

/**
 * 生命周期与打开的文件相同.
 */
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
    PONY_GUARD_BY(FRAME) virtual void statePause() { NOT_IMPLEMENT_YET }

    PONY_GUARD_BY(FRAME) virtual void flush() { NOT_IMPLEMENT_YET }

    PONY_GUARD_BY(FRAME) virtual void stateResume() { NOT_IMPLEMENT_YET }

    PONY_GUARD_BY(FRAME) virtual void seek(qreal secs) { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE virtual VideoFrame getPicture() { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE virtual qreal frontPicture() { NOT_IMPLEMENT_YET }

    virtual int skipPicture(const std::function<bool(qreal)> &function) { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE virtual AudioFrame getSample()  { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE virtual qreal frontSample() { NOT_IMPLEMENT_YET }

    virtual int skipSample(const std::function<bool(qreal)> &function) { NOT_IMPLEMENT_YET }

    virtual void setTrack(int i) { NOT_IMPLEMENT_YET }

    virtual bool hasVideo() { NOT_IMPLEMENT_YET }


    virtual void setEnableAudio(bool enable) { NOT_IMPLEMENT_YET }
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

    PONY_THREAD_SAFE VideoFrame getPicture() override { return videoDecoder->getPicture(); }

    PONY_THREAD_SAFE qreal frontPicture() override { return videoDecoder->viewFront(); }

    PONY_THREAD_SAFE int skipPicture(const std::function<bool(qreal)> &predicate) override { return videoDecoder->skip(predicate); }

    PONY_THREAD_SAFE AudioFrame getSample() override { return audioDecoder->getSample(); }

    PONY_THREAD_SAFE qreal frontSample() override { return audioDecoder->viewFront(); }

    PONY_THREAD_SAFE int skipSample(const std::function<bool(qreal)> &predicate) override { return audioDecoder->skip(predicate); }

    PONY_GUARD_BY(MAIN, FRAME, DECODER) [[nodiscard]] qreal getAudionLength() { return description.audioDuration; }
    PONY_GUARD_BY(MAIN, FRAME, DECODER) [[nodiscard]] qreal getVideoLength() { return description.videoDuration; }

    PONY_GUARD_BY(DECODER) void setTrack(int i) override {
        delete audioDecoder;
        m_audioStreamIndex = description.m_audioStreamsIndex[static_cast<size_t>(i)];
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_lifeManager);
    }

    PONY_GUARD_BY(DECODER) QStringList getTracks() {
        QStringList ret;
        ret.reserve(static_cast<qsizetype>(description.m_audioStreamsIndex.size()));
        for(auto && i : description.m_audioStreamsIndex) {
            ret.emplace_back(description.streamInfos[i].getFriendName());
        }
        return ret;
    }

    PONY_GUARD_BY(DECODER) void setAudioIndex(StreamIndex i) {
        if (i == m_audioStreamIndex) { return; }
        delete audioDecoder;
        m_audioStreamIndex = i;
        audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue, m_lifeManager);
    }

    PONY_GUARD_BY(DECODER) bool hasVideo() {
        return !description.m_videoStreamsIndex.empty() && fmtCtx->streams[m_videoStreamIndex]->nb_frames > 0;
    }

    PONY_GUARD_BY(DECODER) void setEnableAudio(bool enable) {
        audioDecoder->setEnable(false);
    }

private slots:
    void onWork()  {
        videoQueue->open();
        while(!interrupt) {
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
    int audioStreamIndex{-1};

    AVStream *videoStream{};
    AVStream *audioStream{};

    ReverseDecoderImpl<Video> *videoDecoder;
    ReverseDecoderImpl<Audio> *audioDecoder;
    AudioFrame silenceFrame;
    std::vector<StreamInfo> streamInfos;
    std::byte silence[1024]{};

    std::atomic<bool> interrupt = true;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
    qreal m_videoDuration;
    LifeCycleManager *m_lifeManager = new LifeCycleManager;
public:
    explicit ReverseDecodeDispatcher(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent),
    silenceFrame(silence, 1024, std::numeric_limits<double>::max()){
        packet = av_packet_alloc();
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
                videoStreamIndex = i;
                videoStream = fmtCtx->streams[i];
            }
            else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex == -1) {
                audioStreamIndex = i;
                audioStream = fmtCtx->streams[i];
            }
        }

        if (videoStreamIndex < 0) { throw std::runtime_error("Cannot find video stream."); }
        if (audioStreamIndex < 0) { throw std::runtime_error("Cannot find audio stream."); }

        // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
        videoQueue = new TwinsBlockQueue<AVFrame *>("VideoQueue", 16);
        audioQueue = videoQueue->twins("AudioQueue", 16);

        audioDecoder = new ReverseDecoderImpl<Audio>(audioStream, audioQueue, m_lifeManager, nullptr);
        videoDecoder = new ReverseDecoderImpl<Video>(videoStream, videoQueue, m_lifeManager, audioDecoder);
        m_videoDuration = videoDecoder->duration();
        connect(this, &ReverseDecodeDispatcher::signalStartWorker, this, &ReverseDecodeDispatcher::onWork, Qt::QueuedConnection);
    }

    ~ReverseDecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        flush();
        delete audioQueue;
        delete videoQueue;
        delete videoDecoder;
        delete audioDecoder;
        if (packet) { av_packet_free(&packet); }
        m_lifeManager->deleteLater();
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
        auto freeFunc = [](AVFrame * frame){
            if (frame) av_frame_free(&frame);
        };
        videoQueue->clear(freeFunc);
        audioQueue->clear(freeFunc);
        videoDecoder->clearFrameStack();
        audioDecoder->clearFrameStack();
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
        audioDecoder->flushFFmpegBuffers();
        qDebug() << "a Seek:" << secs;
        secs = fmax(secs, 0.0);
        videoDecoder->setStart(secs);
        audioDecoder->setStart(secs);
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>((secs-2.0) * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    PONY_THREAD_SAFE VideoFrame getPicture() override { return videoDecoder->getPicture(); }

    PONY_THREAD_SAFE qreal frontPicture() override { return videoDecoder->viewFront(); }

    PONY_THREAD_SAFE AudioFrame getSample() override { return audioDecoder->getSample(); }

    PONY_THREAD_SAFE qreal frontSample() override { NOT_IMPLEMENT_YET }

    PONY_GUARD_BY(DECODER) void setEnableAudio(bool enable) {
//        audioDecoder->setEnable(false);
    }


    bool hasVideo() override { return true; }

private slots:
    void onWork() {
        videoQueue->open();
        while(!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (packet->stream_index == videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                    // qDebug() << "accept";
                    auto next = videoDecoder->nextSegment();
                    if (next > 0) {
                        //std::cerr << "next: " << next << std::endl;
                        videoDecoder->setStart(next);
                        audioDecoder->setStart(next);
                        videoDecoder->flushFFmpegBuffers();
                        audioDecoder->flushFFmpegBuffers();
                        av_seek_frame(fmtCtx, videoStreamIndex,
                                      static_cast<int64_t>((next-2.0) / av_q2d(videoStream->time_base)),
                                      AVSEEK_FLAG_BACKWARD);
                    }
                    else if (next == 0) {
                        //std::cerr << "reverse: finish" << std::endl;
                        av_packet_unref(packet);
                        videoDecoder->pushEOF();
                        audioDecoder->pushEOF();
                        // qDebug() << "reverse: finish";
                        break;
                    }
                }
                else if (packet->stream_index == audioStreamIndex) {
                    audioDecoder->accept(packet, interrupt);
                }
            } else if (ret == ERROR_EOF) {
                //std::cerr << "reverse reach eof" << std::endl;
                qDebug() << "reverse: reach eof";
                videoDecoder->flushFFmpegBuffers();
                audioDecoder->flushFFmpegBuffers();
                av_seek_frame(fmtCtx, -1,
                              static_cast<int64_t>((m_videoDuration-4.0) * AV_TIME_BASE),
                              AVSEEK_FLAG_BACKWARD);
                videoDecoder->setStart(m_videoDuration-2.0);
                audioDecoder->setStart(m_videoDuration-2.0);
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



