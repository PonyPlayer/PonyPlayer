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
#include "audioformat.hpp"
#include "virtual.hpp"

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

//#define IGNORE_VIDEO_FRAME

namespace PonyPlayer {
    Q_NAMESPACE
    enum OpenFileResultType {
        FAILED,        ///< 打开文件失败
        VIDEO,         ///< 打开的文件为视频文件
        AUDIO          ///< 打开的文件为音频文件
    };

    Q_ENUM_NS(OpenFileResultType)
}


class StreamInfo {
private:
    int index;
    std::unordered_map<std::string, std::string> dict;
    qreal duration;
public:
    explicit StreamInfo(AVStream *stream) : index(stream->index) {
        AVDictionaryEntry *tag = nullptr;
        while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            dict[tag->key] = tag->value;
        }
        duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    [[nodiscard]] qreal getDuration() const { return duration; }

    [[nodiscard]] QString getFriendName() const {
        QString str;
        if (auto iter = dict.find("language"); iter != dict.cend()) {
            str.append(iter->second.c_str());
            str.append(" | ");
        }
        auto minutes = static_cast<int>(duration) / 60;
        auto seconds = duration - minutes * 60;
        if (minutes > 0) { str.append(QString::number(minutes)).append("m"); }
        str.append(QString::number(seconds, 'f', 1).append("s"));
        return str;
    }

    [[nodiscard]] int getIndex() const { return index; }

};

typedef unsigned int StreamIndex;
const constexpr StreamIndex DEFAULT_STREAM_INDEX = std::numeric_limits<StreamIndex>::max();

/**
 * 生命周期与打开的文件相同.
 */
class DemuxDispatcherBase : public QObject {
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
    PONY_GUARD_BY(FRAME)

    virtual void statePause() {NOT_IMPLEMENT_YET}

    PONY_GUARD_BY(FRAME)

    virtual void flush() {NOT_IMPLEMENT_YET}

    PONY_GUARD_BY(FRAME)

    virtual void stateResume() {NOT_IMPLEMENT_YET}

    PONY_GUARD_BY(FRAME)

    virtual void seek(qreal secs) {NOT_IMPLEMENT_YET}

    PONY_THREAD_SAFE virtual VideoFrameRef getPicture() {NOT_IMPLEMENT_YET}

    PONY_THREAD_SAFE virtual qreal frontPicture() {NOT_IMPLEMENT_YET}

    virtual int skipPicture(const std::function<bool(qreal)> &function) {NOT_IMPLEMENT_YET}

    PONY_THREAD_SAFE virtual AudioFrame getSample() {NOT_IMPLEMENT_YET}

    PONY_THREAD_SAFE virtual qreal frontSample() {NOT_IMPLEMENT_YET}

    virtual int skipSample(const std::function<bool(qreal)> &function) {NOT_IMPLEMENT_YET}

    virtual void setTrack(int i) {NOT_IMPLEMENT_YET}

    virtual bool hasVideo() {NOT_IMPLEMENT_YET}

    virtual void setEnableAudio(bool enable) {NOT_IMPLEMENT_YET}

    virtual PonyAudioFormat getAudioInputFormat() = 0;

    virtual void setAudioOutputFormat(PonyAudioFormat format) = 0;

    virtual void test_onWork() = 0;
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
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
    StreamIndex m_audioStreamIndex;
    StreamIndex m_videoStreamIndex;
    IDemuxDecoder *m_audioDecoder;
    IDemuxDecoder *videoDecoder;

    std::atomic<bool> interrupt = true;
    AVPacket *packet = nullptr;

public:
    explicit DecodeDispatcher(
            const std::string &fn,
            PonyPlayer::OpenFileResultType &result,
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
        if (description.m_audioStreamsIndex.empty()) {
            result = PonyPlayer::OpenFileResultType::FAILED;
            throw std::runtime_error("Cannot find audio stream.");
        }
        if (m_audioStreamIndex ==
            DEFAULT_STREAM_INDEX) { m_audioStreamIndex = description.m_audioStreamsIndex.front(); }
        audioQueue = new TwinsBlockQueue<AVFrame *>("AudioQueue", 16);
        m_audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue);
        description.audioDuration = m_audioDecoder->duration();

        // video
        videoQueue = audioQueue->twins("VideoQueue", 16);
        if (description.m_videoStreamsIndex.empty() ||
            fmtCtx->streams[description.m_videoStreamsIndex.front()]->nb_frames == 0) {
            // no video
            qDebug() << "audio only";
            if (!description.m_videoStreamsIndex.empty())
                m_videoStreamIndex = description.m_videoStreamsIndex.front();
            videoDecoder = new VirtualVideoDecoder(description.audioDuration);
            result = PonyPlayer::OpenFileResultType::AUDIO;
        } else {
            result = PonyPlayer::OpenFileResultType::VIDEO;
            if (m_videoStreamIndex ==
                DEFAULT_STREAM_INDEX) { m_videoStreamIndex = description.m_videoStreamsIndex.front(); }
            videoDecoder = new DecoderImpl<Video>(fmtCtx->streams[m_videoStreamIndex], videoQueue);
        }
        description.videoDuration = videoDecoder->duration();
        connect(this, &DecodeDispatcher::signalStartWorker, this, &DecodeDispatcher::onWork, Qt::QueuedConnection);
    }

    ~DecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        DecodeDispatcher::flush();
        delete audioQueue;
        delete videoQueue;
        delete m_audioDecoder;
        delete videoDecoder;
        if (packet) { av_packet_free(&packet); }
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
        videoQueue->clear([](AVFrame *frame) { av_frame_free(&frame); });
        audioQueue->clear([](AVFrame *frame) { av_frame_free(&frame); });
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
        if (m_audioDecoder) { m_audioDecoder->flushFFmpegBuffers(); }
        if (videoDecoder) { videoDecoder->flushFFmpegBuffers(); }
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    PONY_THREAD_SAFE VideoFrameRef getPicture() override { return videoDecoder->getPicture(); }

    PONY_THREAD_SAFE qreal frontPicture() override { return videoDecoder->viewFront(); }

    PONY_THREAD_SAFE int skipPicture(const std::function<bool(qreal)> &predicate) override {
        return videoDecoder->skip(predicate);
    }

    PONY_THREAD_SAFE AudioFrame getSample() override { return m_audioDecoder->getSample(); }

    PONY_THREAD_SAFE qreal frontSample() override { return m_audioDecoder->viewFront(); }

    PONY_THREAD_SAFE int skipSample(const std::function<bool(qreal)> &predicate) override {
        return m_audioDecoder->skip(predicate);
    }

    PONY_GUARD_BY(MAIN, FRAME, DECODER)

    [[nodiscard]] qreal getAudionLength() const { return description.audioDuration; }

    PONY_GUARD_BY(MAIN, FRAME, DECODER)

    [[nodiscard]] qreal getVideoLength() const { return description.videoDuration; }

    PONY_GUARD_BY(DECODER)

    void setTrack(int i) override {
        delete m_audioDecoder;
        m_audioStreamIndex = description.m_audioStreamsIndex[static_cast<size_t>(i)];
        auto stream = fmtCtx->streams[m_audioStreamIndex];
        m_audioDecoder = new DecoderImpl<Audio>(stream, audioQueue);
    }

    PONY_GUARD_BY(DECODER)

    QStringList getTracks() {
        QStringList ret;
        ret.reserve(static_cast<qsizetype>(description.m_audioStreamsIndex.size()));
        for (auto &&i: description.m_audioStreamsIndex) {
            ret.emplace_back(description.streamInfos[i].getFriendName());
        }
        return ret;
    }

    PONY_GUARD_BY(DECODER)

    void setAudioIndex(StreamIndex i) {
        if (i == m_audioStreamIndex) { return; }
        delete m_audioDecoder;
        m_audioStreamIndex = i;
        m_audioDecoder = new DecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue);
    }

    PONY_GUARD_BY(DECODER)

    bool hasVideo() override {
        return !description.m_videoStreamsIndex.empty() && fmtCtx->streams[m_videoStreamIndex]->nb_frames > 0;
    }

    PONY_GUARD_BY(DECODER)

    void setEnableAudio(bool enable) override {
        m_audioDecoder->setEnable(enable);
    }

    PonyAudioFormat getAudioInputFormat() override {
        return m_audioDecoder->getInputFormat();
    }

    void setAudioOutputFormat(PonyAudioFormat format) override {
        m_audioDecoder->setOutputFormat(format);
    }

    void test_onWork() override {
        onWork();
    }

private slots:

    void onWork() {
        videoQueue->open();
        while (!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (static_cast<StreamIndex>(packet->stream_index) == m_videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                } else if (static_cast<StreamIndex>(packet->stream_index) == m_audioStreamIndex) {
                    m_audioDecoder->accept(packet, interrupt);
                }
            } else if (ret == ERROR_EOF) {
                videoQueue->push(nullptr);
                audioQueue->push(nullptr);
                av_packet_unref(packet);
                break;
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
    struct {
        qreal videoDuration = std::numeric_limits<qreal>::quiet_NaN();
        qreal audioDuration = std::numeric_limits<qreal>::quiet_NaN();
        std::vector<StreamIndex> m_videoStreamsIndex;
        std::vector<StreamIndex> m_audioStreamsIndex;
        std::vector<StreamInfo> streamInfos;
    } description;

    StreamIndex m_audioStreamIndex;
    StreamIndex m_videoStreamIndex;

    const qreal interval = 5.0;

    AVStream *videoStream{};
    AVStream *audioStream{};

    IDemuxDecoder *videoDecoder;
    IDemuxDecoder *m_audioDecoder;
    IDemuxDecoder *primary;

    std::atomic<bool> interrupt = true;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
public:
    explicit ReverseDecodeDispatcher(const std::string &fn,
                                     QObject *parent = nullptr
    ) : DemuxDispatcherBase(fn, parent), m_audioStreamIndex(DEFAULT_STREAM_INDEX), m_videoStreamIndex(DEFAULT_STREAM_INDEX) {
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
        if (description.m_audioStreamsIndex.empty()) {
            throw std::runtime_error("Cannot find audio stream.");
        }
        if (m_audioStreamIndex ==
            DEFAULT_STREAM_INDEX) { m_audioStreamIndex = description.m_audioStreamsIndex.front(); }

        audioQueue = new TwinsBlockQueue<AVFrame *>("AudioQueue", 200);

        m_audioDecoder = new ReverseDecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue);
        description.audioDuration = m_audioDecoder->duration();

        // video
        videoQueue = audioQueue->twins("VideoQueue", 200);
        if (description.m_videoStreamsIndex.empty() ||
            fmtCtx->streams[description.m_videoStreamsIndex.front()]->nb_frames == 0) {
            // no video
            qDebug() << "audio only";
            if (!description.m_videoStreamsIndex.empty())
                m_videoStreamIndex = description.m_videoStreamsIndex.front();
            videoDecoder = new VirtualVideoDecoder(description.audioDuration);
            videoQueue->setEnable(false);
            m_audioDecoder->setFollower(m_audioDecoder);
            primary = m_audioDecoder;
        } else {
            if (m_videoStreamIndex ==
                DEFAULT_STREAM_INDEX) { m_videoStreamIndex = description.m_videoStreamsIndex.front(); }
            videoDecoder = new ReverseDecoderImpl<Video>(fmtCtx->streams[m_videoStreamIndex], videoQueue);
            videoDecoder->setFollower(m_audioDecoder);
            primary = videoDecoder;
        }
        description.videoDuration = videoDecoder->duration();
        /*
        videoQueue = new TwinsBlockQueue<AVFrame *>("VideoQueue", 200);
        audioQueue = videoQueue->twins("AudioQueue", 200);

        m_audioDecoder = new ReverseDecoderImpl<Audio>(audioStream, audioQueue);
        description.audioDuration = m_audioDecoder->duration();

        if (videoStreamIndex >= 0 && videoStream->nb_frames > 0) {
            qDebug() << "normal reverse";
            videoDecoder = new ReverseDecoderImpl<Video>(videoStream, videoQueue);
            videoDecoder->setFollower(m_audioDecoder);
            m_videoDuration = videoDecoder->duration();
            primary = videoDecoder;
        } else {
            qDebug() << "audio only reverse";
            m_videoDuration = m_audioDecoder->duration();
            videoDecoder = new VirtualVideoDecoder(m_videoDuration);
            videoQueue->setEnable(false);
            m_audioDecoder->setFollower(m_audioDecoder);
            primary = m_audioDecoder;
        }
        */
        connect(this, &ReverseDecodeDispatcher::signalStartWorker, this, &ReverseDecodeDispatcher::onWork,
                Qt::QueuedConnection);
    }

    ~ReverseDecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        ReverseDecodeDispatcher::flush();
        delete audioQueue;
        delete videoQueue;
        delete m_audioDecoder;
        delete videoDecoder;
        if (packet) { av_packet_free(&packet); }
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
        auto freeFunc = [](AVFrame *frame) {
            if (frame) av_frame_free(&frame);
        };
        videoQueue->clear(freeFunc);
        audioQueue->clear(freeFunc);
        videoDecoder->clearFrameStack();
        m_audioDecoder->clearFrameStack();
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
    PONY_GUARD_BY(DECODER)

    void seek(qreal secs) override {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        videoDecoder->flushFFmpegBuffers();
        m_audioDecoder->flushFFmpegBuffers();
        qDebug() << "a Seek:" << secs;
        secs = fmax(secs, 0.0);
        videoDecoder->setStart(secs);
        m_audioDecoder->setStart(secs);
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>((secs - interval) * AV_TIME_BASE),
                                AVSEEK_FLAG_BACKWARD);
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    PONY_THREAD_SAFE VideoFrameRef getPicture() override { return videoDecoder->getPicture(); }

    PONY_THREAD_SAFE qreal frontPicture() override { return videoDecoder->viewFront(); }

    PONY_THREAD_SAFE AudioFrame getSample() override { return m_audioDecoder->getSample(); }

    PONY_THREAD_SAFE qreal frontSample() override {NOT_IMPLEMENT_YET}

    PONY_GUARD_BY(DECODER)

    void setEnableAudio(bool enable) override { m_audioDecoder->setEnable(enable); }

    PonyAudioFormat getAudioInputFormat() override { // TODO: IMPLEMENT LATER
        return m_audioDecoder->getInputFormat();
    }

    void setAudioOutputFormat(PonyAudioFormat format) override { // TODO: IMPLEMENT LATER
        m_audioDecoder->setOutputFormat(format);
    }

    bool hasVideo() override {
        return !description.m_videoStreamsIndex.empty() && fmtCtx->streams[m_videoStreamIndex]->nb_frames > 0;
    }

    void test_onWork() override {
        onWork();
    }

    PONY_GUARD_BY(DECODER)
    void setTrack(int i) override {
        delete m_audioDecoder;
        m_audioStreamIndex = description.m_audioStreamsIndex[static_cast<size_t>(i)];
        auto stream = fmtCtx->streams[m_audioStreamIndex];
        m_audioDecoder = new ReverseDecoderImpl<Audio>(stream, audioQueue);
        if (hasVideo())
            videoDecoder->setFollower(m_audioDecoder);
        else {
            m_audioDecoder->setFollower(m_audioDecoder);
            primary = m_audioDecoder;
        }
    }

    PONY_GUARD_BY(DECODER)
    QStringList getTracks() {
        QStringList ret;
        ret.reserve(static_cast<qsizetype>(description.m_audioStreamsIndex.size()));
        for (auto &&i: description.m_audioStreamsIndex) {
            ret.emplace_back(description.streamInfos[i].getFriendName());
        }
        return ret;
    }

    PONY_GUARD_BY(DECODER)
    void setAudioIndex(StreamIndex i) {
        if (i == m_audioStreamIndex) { return; }
        delete m_audioDecoder;
        m_audioStreamIndex = i;
        m_audioDecoder = new ReverseDecoderImpl<Audio>(fmtCtx->streams[m_audioStreamIndex], audioQueue);
        if (hasVideo())
            videoDecoder->setFollower(m_audioDecoder);
        else {
            m_audioDecoder->setFollower(m_audioDecoder);
            primary = m_audioDecoder;
        }
    }

private slots:

    void onWork() {
        videoQueue->open();
        while (!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                if (static_cast<StreamIndex>(packet->stream_index) == m_videoStreamIndex) {
                    videoDecoder->accept(packet, interrupt);
                } else if (static_cast<StreamIndex>(packet->stream_index) == m_audioStreamIndex) {
                    m_audioDecoder->accept(packet, interrupt);
                }
            } else if (ret == ERROR_EOF) {
                //std::cerr << "reverse reach eof" << std::endl;
                qDebug() << "reverse: reach eof";
                videoDecoder->flushFFmpegBuffers();
                m_audioDecoder->flushFFmpegBuffers();
                av_seek_frame(fmtCtx, -1,
                              static_cast<int64_t>((description.audioDuration - 2 * interval) * AV_TIME_BASE),
                              AVSEEK_FLAG_BACKWARD);
                videoDecoder->setStart(description.audioDuration - interval);
                m_audioDecoder->setStart(description.audioDuration - interval);
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            av_packet_unref(packet);
            auto next = primary->nextSegment();
            if (next > 0) {
                //std::cerr << "next: " << next << std::endl;
                videoDecoder->setStart(next);
                m_audioDecoder->setStart(next);
                videoDecoder->flushFFmpegBuffers();
                m_audioDecoder->flushFFmpegBuffers();
                av_seek_frame(fmtCtx, -1,
                              static_cast<int64_t>((next - interval) * AV_TIME_BASE),
                              AVSEEK_FLAG_BACKWARD);
            } else if (next == 0) {
                //std::cerr << "reverse: finish" << std::endl;
                av_packet_unref(packet);
                videoQueue->push(nullptr);
                audioQueue->push(nullptr);
                qDebug() << "reverse: reach starting pointing";
                break;
            }
        }
        interrupt = true;
    };

signals:

    void signalStartWorker(QPrivateSignal);
};




