//
// Created by ColorsWind on 2022/4/30.
// Adapted from demuxer v1 by kurisu on 2022/4/16.
//
#pragma once

#include <QtCore>
#include <QTimer>
#include "helper.hpp"
#include "frame.hpp"
#include "../utils/include/twins_queue.hpp"
#include "decoder.hpp"

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

//#define IGNORE_VIDEO_FRAME




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
};




/**
 * @brief 解码器调度器, 将Packet分配给解码器进一步解码成Frame
 * 这个类是RAII的
 */
class DecodeDispatcher : public DemuxDispatcherBase {
    Q_OBJECT
    std::vector<unsigned int> videoStreamIndex;
    std::vector<unsigned int> audioStreamIndex;
    std::vector<IDemuxDecoder*> decoders;
    std::atomic<bool> interrupt = false;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
    FrameFreeFunc *m_freeFunction;
    FrameFreeQueue *m_freeQueue;

public:
    explicit DecodeDispatcher(
            const std::string &fn,
            FrameFreeQueue *freeQueue,
            FrameFreeFunc *freeFunc,
            QObject *parent
    ) : DemuxDispatcherBase(fn, parent), m_freeQueue(freeQueue), m_freeFunction(freeFunc) {
        packet = av_packet_alloc();
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex.emplace_back(i);
            } else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audioStreamIndex.emplace_back(i);
            }
        }
        if (videoStreamIndex.empty()) { throw std::runtime_error("Cannot find video stream."); }
        if (audioStreamIndex.empty()) { throw std::runtime_error("Cannot find audio stream."); }
        decoders.resize(fmtCtx->nb_streams);
        // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
        videoQueue = new TwinsBlockQueue<AVFrame *>("VideoQueue", 16);
        audioQueue = videoQueue->twins("AudioQueue", 16);
        decoders[videoStreamIndex.front()] = new DecoderImpl<Video>(fmtCtx->streams[videoStreamIndex.front()], videoQueue, freeQueue, freeFunc);
        decoders[audioStreamIndex.front()] = new DecoderImpl<Audio>(fmtCtx->streams[audioStreamIndex.front()], audioQueue, freeQueue, freeFunc);


        interrupt = false; // happens before
    }

    ~DecodeDispatcher() override {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        AVFrame *frame;
        while(m_freeQueue->try_dequeue(frame)) { av_frame_free(&frame); }
        flush();
        if (videoQueue) { videoQueue->close(); }
        if (audioQueue) { audioQueue->close(); }
        for(auto && decoder : decoders) { delete decoder; }
        if(packet) { av_packet_free(&packet); }

    }

    /**
     * 保证阻塞获取结果的线程尽快被唤醒, 同时请求 DecodeThread 的工作尽快停止.
     */
    void statePause() {
        interrupt = true;
        videoQueue->close();
        qDebug() << "Queue close.";
    }

    void flush() {
        videoQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
        audioQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
    }


    /**
     * 保证可以阻塞地获取 Picture 和 Sample. 这个方法是线程安全的.
     */
    void stateResume() {
        interrupt = false;
        videoQueue->open();
        qDebug() << "Queue stateResume.";
    }

    /**
     * 修改视频播放进度, 注意: 这个方法必须在解码线程上调用.
     * @param secs 新的视频进度(单位: 秒)
     */
    void seek(qreal secs) {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        for (const auto & decoder : decoders) {
            // interrupt
            if (decoder) decoder->flushFFmpegBuffers();
        }
        qDebug() << "a Seek:" << secs;
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(secs * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }
    }

    VideoFrame getPicture(bool b, bool own) { return decoders[videoStreamIndex.front()]->getPicture(b, own); }

    bool popPicture(bool b) { return decoders[videoStreamIndex.front()]->pop(b); }

    AudioFrame getSample(bool b) { return decoders[audioStreamIndex.front()]->getSample(b); }

    bool popSample(bool b) { return decoders[audioStreamIndex.front()]->pop(b); }

    qreal getAudionLength() { return decoders[audioStreamIndex.front()]->duration(); }
    qreal getVideoLength() { return decoders[videoStreamIndex.front()]->duration(); }
public slots:
    void onWork() {
        videoQueue->open();
        while(!interrupt) {
            AVFrame *frame;
            while(m_freeQueue->try_dequeue(frame)) { av_frame_free(&frame); }
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                auto *decoder = decoders[static_cast<unsigned long>(packet->stream_index)];
                if (decoder) { decoder->accept(packet, interrupt); }
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
};


