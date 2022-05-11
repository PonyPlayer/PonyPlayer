//
// Created by ColorsWind on 2022/5/1.
//
#pragma once
#include "helper.hpp"
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END
#include <QDEbug>
#include "frame.hpp"
#include "twins_queue.hpp"
#include "concurrentqueue.h"
#include <atomic>
#include <utility>

typedef moodycamel::ConcurrentQueue<AVFrame *> FrameFreeQueue;

class IDemuxDecoder {

public:
    /**
     * 解码器类型
     */
    enum class DecoderType {
        Audio,  ///< 音频解码器
        Video,  ///< 视频解码器
        Common, ///< 未指定
    };
    /**
     * 接收一个包
     * @param pkt
     * @return 如果还需要接收下一个 packet 返回 true, 否则返回 false
     */
    virtual bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) = 0;

    /**
     * 清空 FFmpeg 内部缓冲区
     */
    virtual void flushFFmpegBuffers() = 0;


    /**
     * 获取视频帧, 仅当当前解码器是视频解码器时有效
     * @param b 是否阻塞
     * @return 视频帧, 请用 isValid 判断是否有效
     */
    virtual VideoFrame getPicture(bool b, bool own) = 0;

    /**
    * 获取音频帧, 仅当当前解码器是音频解码器时有效
    * @param b 是否阻塞
    * @return 音频帧, 请用 isValid 判断是否有效
    */
    virtual AudioFrame getSample(bool b) = 0;

    /**
     * 从队列中删除视频帧/音频帧
     * @param b 是否阻塞
     * @return 是否成功
     */
    virtual bool pop(bool b) = 0;

    /**
     * 获取流的长度
     * @return
     */
    virtual double duration() = 0;

    virtual ~IDemuxDecoder() = default;

};

class DecoderContext {
public:
    AVCodec *codec = nullptr;
    AVStream *stream = nullptr;
    AVCodecContext *codecCtx = nullptr;
    AVFrame *frameBuf = nullptr;
    DecoderContext(AVStream *vs): stream(vs) {
        auto *videoCodecPara = stream->codecpar;
        if (!(codec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecPara->codec_id)))) {
            throw std::runtime_error("Cannot find valid video decode codec.");
        }
        if (!(codecCtx = avcodec_alloc_context3(codec))) {
            throw std::runtime_error("Cannot find valid video decode codec context.");
        }
        if (avcodec_parameters_to_context(codecCtx, videoCodecPara) < 0) {
            throw std::runtime_error("Cannot initialize videoCodecCtx.");
        }
        if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
            throw std::runtime_error("Cannot open codec.");
        }
        if (!(frameBuf = av_frame_alloc())) {
            throw std::runtime_error("Cannot alloc frame buf.");
        }
    }

    ~DecoderContext() {
        if (frameBuf) { av_frame_free(&frameBuf); }
        if (codecCtx) { avcodec_close(codecCtx); }
        if (codecCtx) { avcodec_free_context(&codecCtx); }
    }

};


using IDemuxDecoder::DecoderType::Audio;
using IDemuxDecoder::DecoderType::Video;
using IDemuxDecoder::DecoderType::Common;

template<IDemuxDecoder::DecoderType type>
class DecoderImpl : public DecoderContext, public IDemuxDecoder {
protected:
    TwinsBlockQueue<AVFrame *> *frameQueue;
    FrameFreeFunc *m_freeFunc;
    FrameFreeQueue *m_freeQueue;

public:
    DecoderImpl(
            AVStream *vs,
            TwinsBlockQueue<AVFrame *> *queue,
            FrameFreeQueue *freeQueue,
            FrameFreeFunc *freeFunc
    ) : DecoderContext(vs), frameQueue(queue), m_freeFunc(freeFunc), m_freeQueue(freeQueue) {}

    double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0 && !interrupt) {
            ret = avcodec_receive_frame(codecCtx, frameBuf);
            if (ret >= 0) {
#ifdef IGNORE_VIDEO_FRAME
                if constexpr(type == Video) {
                if (frameQueue.getSize() > 10) {
                    av_frame_unref(frameBuf);
                    av_frame_free(&frameBuf);
                } else {
                    ret = frameQueue.bpush(frameBuf);
                }
            } else {
                ret = frameQueue.bpush(frameBuf);
            }
#else
                frameQueue->push(frameBuf);
#endif
                frameBuf = av_frame_alloc();
            } else if (ret == AVERROR(EAGAIN)) {
                return true;
            } else if (ret == ERROR_EOF) {
                frameQueue->push(nullptr);
                return false;
            } else {
                frameQueue->push(nullptr);
                qWarning() << "Error avcodec_receive_frame:" << ffmpegErrToString(ret);
                return false;
            }
        }
        return false;
    }

    VideoFrame getPicture(bool b, bool own) override {
        throw std::runtime_error("Unsupported operation.");
    }

    AudioFrame getSample(bool b) override {
        throw std::runtime_error("Unsupported operation.");
    }

    bool pop(bool b) override {
        return frameQueue->pop();
    }

    void flushFFmpegBuffers() override {
        avcodec_flush_buffers(codecCtx);
    }

};

/**
 * 音频解码器实现
 */
template<> class DecoderImpl<Audio>: public DecoderImpl<Common> {
    SwrContext *swrCtx = nullptr;
    uint8_t *audioOutBuf = nullptr;
    AVFrame * sampleFrameBuf = nullptr;

public:
    DecoderImpl(
            AVStream *vs,
            TwinsBlockQueue<AVFrame *> *queue,
            FrameFreeQueue *freeQueue,
            FrameFreeFunc *freeFunc
    ) : DecoderImpl<Common>(vs, queue, freeQueue, freeFunc) {
        this->swrCtx = swr_alloc_set_opts(swrCtx, av_get_default_channel_layout(2),
                                          AV_SAMPLE_FMT_S16, 44100,
                                          static_cast<int64_t>(codecCtx->channel_layout), codecCtx->sample_fmt,
                                          codecCtx->sample_rate, 0, nullptr);

        if (!swrCtx || swr_init(swrCtx) < 0) {
            throw std::runtime_error("Cannot initialize swrCtx");
        }
        if (!(audioOutBuf = static_cast<uint8_t *>(av_malloc(2 * MAX_AUDIO_FRAME_SIZE)))) {
            throw std::runtime_error("Cannot alloc audioOutBuf");
        }
        sampleFrameBuf = av_frame_alloc();
    }

    virtual ~DecoderImpl() {
        if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
        if (audioOutBuf) { av_freep(&audioOutBuf); }
        if (swrCtx) { swr_free(&swrCtx); }
    }


    AudioFrame getSample(bool b) override {
        AVFrame *frame = frameQueue->front();
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(nullptr, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        return {reinterpret_cast<std::byte *>(audioOutBuf), out_size, pts, frame};
    }

    bool pop(bool b) override {
        AVFrame *frame = frameQueue->front();
        m_freeQueue->enqueue(frame);
        return frameQueue->pop();
    }
};

/**
 * 视频解码器实现
 */
template<> class DecoderImpl<Video>: public DecoderImpl<Common> {
public:
    DecoderImpl(
            AVStream *vs,
            TwinsBlockQueue<AVFrame *> *queue,
            FrameFreeQueue *freeQueue,
            FrameFreeFunc *freeFunc
    ) : DecoderImpl<Common>(vs, queue, freeQueue, freeFunc) {}



    VideoFrame getPicture(bool b, bool own) override {
        AVFrame *frame = frameQueue->front();
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, pts, own ? *m_freeFunc : nullptr};
    }

    bool pop(bool b) override {
        bool ret = DecoderImpl<Common>::pop(b);
        return ret;
    }

};

#include <iostream>
/**
 * 反向Decoder
 * @tparam type
 */
template<IDemuxDecoder::DecoderType type>
class ReverseDecoderImpl : public DecoderContext, public IDemuxDecoder {
protected:
    TwinsBlockQueue<std::vector<AVFrame *>*> *frameQueue;
    std::vector<AVFrame*> *frameStack;
    qreal from;
    qreal next{-1.0};

public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<std::vector<AVFrame *>*> *queue) :
        DecoderContext(vs), frameQueue(queue), frameStack(new std::vector<AVFrame*>) {
        from = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    ~ReverseDecoderImpl() {
        if (frameStack) {
            for (auto frame: *frameStack) {
                av_frame_free(&frame);
            }
            delete frameStack;
        }
    }

    void setStart(qreal secs) {
        from = secs;
        for (auto frame : *frameStack) {
            av_frame_free(&frame);
        }
        frameStack->clear();
    }

    qreal nextSegment() {
        auto res = next;
        next = -1.0;
        return res;
    }

    bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        AVFrame *frame;
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0 && !interrupt) {
            ret = avcodec_receive_frame(codecCtx, frameBuf);
            if (ret >= 0) {
#ifdef IGNORE_VIDEO_FRAME
                if constexpr(type == Video) {
                if (frameQueue.getSize() > 10) {
                    av_frame_unref(frameBuf);
                    av_frame_free(&frameBuf);
                } else {
                    ret = frameQueue.bpush(frameBuf);
                }
            } else {
                ret = frameQueue.bpush(frameBuf);
            }
#else
                double pts = av_q2d(stream->time_base) * static_cast<double>(frameBuf->pts);
                if (pts < from-1.0) {
                    av_frame_unref(frameBuf);
                    continue;
                }
                else if (pts <= from){
                    //std::cerr << pts << " " << from << std::endl;
                    frameStack->push_back(frameBuf);
                }
                else {
                    //std::cerr << "push to queue"<< std::endl;
                    av_frame_unref(frameBuf);
                    frameQueue->push(frameStack);
                    frameStack = new std::vector<AVFrame*>;
                    if (from < 1.0)
                        from = 0;
                    else
                        from -= 1.0;
                    next = from;
                    return true;
                }
#endif
                frameBuf = av_frame_alloc();
            } else if (ret == AVERROR(EAGAIN)) {
                return true;
            } else if (ret == ERROR_EOF) {
                return false;
            } else {
                qWarning() << "Error avcodec_receive_frame:" << ffmpegErrToString(ret);
                return false;
            }
        }
        return false;
    }

    VideoFrame getPicture(bool b, bool own) override {
        throw std::runtime_error("Unsupported operation.");
    }

    AudioFrame getSample(bool b) override {
        throw std::runtime_error("Unsupported operation.");
    }

    bool pop(bool b) override {
        return frameQueue->pop();
    }

    void flushFFmpegBuffers() override {
        avcodec_flush_buffers(codecCtx);
    }

};

/**
 * 视频解码器实现
 */
template<> class ReverseDecoderImpl<Video>: public ReverseDecoderImpl<Common> {
private:
    FrameFreeFunc freeFunc;
public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<std::vector<AVFrame *>*> *queue) : ReverseDecoderImpl<Common>(vs, queue) {
        freeFunc = [&](AVFrame* frame){av_frame_free(&frame);};
    }

    VideoFrame getPicture(bool b, bool own) override {
        auto stk = frameQueue->front();
        if (!stk) { return {}; }
        auto frame = stk->back();
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, pts, own ? freeFunc : nullptr};
    }

    bool pop(bool b) override {
        auto stk = frameQueue->front();
        if (stk) {
            stk->pop_back();
            if (stk->empty()) {
                ReverseDecoderImpl<Common>::pop(b);
                delete stk;
            }
        }
        return true;
    }

};

