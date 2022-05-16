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
#include <QDebug>
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
     * 获取视频帧并从队列中删除, 仅当当前解码器是视频解码器时有效
     * @param b 是否阻塞
     * @return 视频帧, 请用 isValid 判断是否有效
     */
    virtual VideoFrame getPicture() = 0;

    /**
    * 获取音频帧并从队列中删除, 仅当当前解码器是音频解码器时有效
    * @param b 是否阻塞
    * @return 音频帧, 请用 isValid 判断是否有效
    */
    virtual AudioFrame getSample() = 0;

    /**
     * 获取队首帧的PTS, 若不存在, 返回NaN
     * @return 队首帧的PTS
     */
    virtual qreal viewFront() = 0;

    /**
     * 线性扫描移除满足条件的帧, 当发现帧不满足条件时, 结束扫描
     * @param predicate 条件
     * @return 移除帧的个数
     */
    virtual int skip(const std::function<bool(qreal)> &predicate) = 0;

    /**
     * 获取流的长度
     * @return
     */
    virtual double duration() = 0;


    virtual void setEnable(bool b) = 0;

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
    LifeCycleManager *m_lifeCycleManager;
public:
    DecoderImpl(
      AVStream *vs,
      TwinsBlockQueue<AVFrame *> *queue,
      LifeCycleManager *lifeCycleManager
    ) : DecoderContext(vs), frameQueue(queue), m_lifeCycleManager(lifeCycleManager) {}

    PONY_THREAD_SAFE double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    PONY_GUARD_BY(DECODER) bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0 && !interrupt) {
            ret = avcodec_receive_frame(codecCtx, frameBuf);
            if (ret >= 0) {
                frameQueue->push(frameBuf);
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

    PONY_THREAD_SAFE VideoFrame getPicture() override { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE AudioFrame getSample() override { NOT_IMPLEMENT_YET }


    PONY_THREAD_SAFE qreal viewFront() override {
        return frameQueue->viewFront<qreal>([this](AVFrame * frame) {
            if (frame) {
                return static_cast<qreal>(frame->pts) * av_q2d(stream->time_base);
            } else {
                return std::numeric_limits<qreal>::quiet_NaN();
            }
        });
    }

    PONY_THREAD_SAFE int skip(const std::function<bool(qreal)> &predicate) override {
        return frameQueue->skip([this, predicate](AVFrame *frame){
           return frame && predicate(static_cast<qreal>(frame->pts) * av_q2d(stream->time_base));
        }, m_lifeCycleManager->freeDirectFunc);
    }

    PONY_THREAD_SAFE void setEnable(bool b) override {
        frameQueue->setEnable(b);
    }

    PONY_GUARD_BY(DECODER) void flushFFmpegBuffers() override {
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
      LifeCycleManager *lifeCycleManager
    ) : DecoderImpl<Common>(vs, queue, lifeCycleManager) {
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

    virtual ~DecoderImpl() override {
        if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
        if (audioOutBuf) { av_freep(&audioOutBuf); }
        if (swrCtx) { swr_free(&swrCtx); }
    }


    PONY_THREAD_SAFE AudioFrame getSample() override {
        AVFrame *frame = frameQueue->remove(true);
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(nullptr, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        m_lifeCycleManager->pop();
        m_lifeCycleManager->freeFrame(frame);
        return {reinterpret_cast<std::byte *>(audioOutBuf), out_size, pts};
    }

};

/**
 * 视频解码器实现
 */
template<> class DecoderImpl<Video>: public DecoderImpl<Common> {
private:
    std::atomic<AVFrame *> stillVideoFrame = nullptr;
public:
    DecoderImpl(
            AVStream *vs,
            TwinsBlockQueue<AVFrame *> *queue,
            LifeCycleManager *lifeCycleManager
    ) : DecoderImpl<Common>(vs, queue, lifeCycleManager) {}


    VideoFrame getPicture() override {
        if (stillVideoFrame != nullptr) { return {stillVideoFrame, -1, nullptr}; }
        AVFrame *frame = frameQueue->remove(true);
        if (!frame) { return {}; }
        m_lifeCycleManager->pop();
        if (frame->pts < 0) {
            stillVideoFrame = frame;
            return {stillVideoFrame, -9, nullptr};
        } else {
            double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
            return {frame, pts, &m_lifeCycleManager->freeFunc};
        }
    }


    ~DecoderImpl() override {
        m_lifeCycleManager->freeLater(stillVideoFrame);
    }


};

/**
 * @brief 虚拟视频播放, 用于视频纯音频文件.
 */
class VirtualVideoDecoder : public IDemuxDecoder {
private:
    qreal m_audioDuration;
public:
    VirtualVideoDecoder(qreal audioDuration) : m_audioDuration(audioDuration) {}

    PONY_THREAD_SAFE bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        return !interrupt;
    }

    PONY_THREAD_SAFE void flushFFmpegBuffers() override {}

    PONY_THREAD_SAFE VideoFrame getPicture() override {
        return {nullptr, std::numeric_limits<qreal>::quiet_NaN(), nullptr, true};
    }

    PONY_THREAD_SAFE AudioFrame getSample() override {
        NOT_IMPLEMENT_YET
    }

    PONY_THREAD_SAFE double duration() override {
        return m_audioDuration;
    }

    PONY_THREAD_SAFE qreal viewFront() override {
        return std::numeric_limits<qreal>::quiet_NaN();
    }

    PONY_THREAD_SAFE int skip(const std::function<bool(qreal)> &predicate) override {
        return 0;
    }

    PONY_THREAD_SAFE void setEnable(bool b) override {}


};

/**
 * 反向Decoder
 * @tparam type
 */
#include <iostream>
template<IDemuxDecoder::DecoderType type>
class ReverseDecoderImpl : public DecoderContext, public IDemuxDecoder {
protected:
    LifeCycleManager *m_lifeCycleManager;
    TwinsBlockQueue<std::vector<AVFrame *>*> *frameQueue;
    std::vector<AVFrame*> *frameStack;
    ReverseDecoderImpl<Common>* follower{};
    qreal lastPts{-1.0};
    qreal from;
    qreal next{-1.0};

public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<std::vector<AVFrame *>*> *queue,
                       LifeCycleManager *lifeCycleManager, ReverseDecoderImpl<Common>* another) :
        DecoderContext(vs), m_lifeCycleManager(lifeCycleManager),
        frameQueue(queue), frameStack(new std::vector<AVFrame*>) {
        follower = another;
        from = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    void pushFrameStack() {
        if (!frameStack->empty()) {
            frameQueue->push(frameStack);
            frameStack = new std::vector<AVFrame *>;
        }
    }

    double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    qreal getLastPts() {
        return lastPts;
    }

    ~ReverseDecoderImpl() override {
        if (frameStack) {
            for (auto frame: *frameStack) {
                if (frame)
                    av_frame_free(&frame);
            }
            delete frameStack;
        }
    }

    void setStart(qreal secs) {
        from = secs;
        lastPts = -1.0;
        for (auto frame : *frameStack) {
            if (frame)
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
                lastPts = av_q2d(stream->time_base) * static_cast<double>(frameBuf->pts);
                if (lastPts < from-1.0) {
                    av_frame_unref(frameBuf);
                    continue;
                }
                else if (lastPts >= from){
                    //std::cerr << "push to queue"<< std::endl;
                    //qDebug() << "push to queue";
                    av_frame_unref(frameBuf);
                    // 只有leader有权决定跳转
                    if (follower && follower->getLastPts() >= from) {
                        //std::cerr << "push to queue"<< std::endl;
                        pushFrameStack();
                        follower->pushFrameStack();
                        if (from < 1.0)
                            from = 0;
                        else
                            from -= 1.0;
                        next = from;
                        return true;
                    }
                }
                else {
                    /*
                    if (follower) {
                        // std::cerr << "video frame: " <<  lastPts << std::endl;
                    }
                    else
                        std::cerr << "audio frame: "<< lastPts << std::endl;
                        */
                    frameStack->push_back(frameBuf);
                    frameBuf = av_frame_alloc();
                }
#endif
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

    VideoFrame getPicture() override {
        NOT_IMPLEMENT_YET
    }

    AudioFrame getSample() override {
        NOT_IMPLEMENT_YET
    }

    void flushFFmpegBuffers() override {
        avcodec_flush_buffers(codecCtx);
    }

    PONY_THREAD_SAFE void setEnable(bool b) override {
        NOT_IMPLEMENT_YET
    }

    qreal viewFront() override {
        NOT_IMPLEMENT_YET
    }

    int skip(const std::function<bool(qreal)> &predicate) override {
        NOT_IMPLEMENT_YET
    }
};

/**
 * 视频解码器实现
 */
template<> class ReverseDecoderImpl<Video>: public ReverseDecoderImpl<Common> {
public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<std::vector<AVFrame *>*> *queue,
                       LifeCycleManager *lifeCycleManager, ReverseDecoderImpl<Common>* follower)
    : ReverseDecoderImpl<Common>(vs, queue, lifeCycleManager, follower) {}

    void pushEOF() {
        std::cerr << "pushEOF";
        frameStack->push_back(nullptr);
        frameQueue->push(frameStack);
        frameStack = nullptr;
    }

    VideoFrame getPicture() override {
        //std::cerr << "video get picture"<< std::endl;
        auto stk = frameQueue->viewFront<std::vector<AVFrame *>*>([](std::vector<AVFrame *>* stk){
            return stk;
        });
        if (!stk) return {};
        if (stk->empty()) {
            throw std::runtime_error("reverse getPicture");
        }
        auto frame = stk->back();
        stk->pop_back();
        if (stk->empty()) {
            delete stk;
            frameQueue->remove(true);
        }
        if (!frame) {
            qDebug() << "Reverse: get EOF Frame";
            return {};
        }
        m_lifeCycleManager->pop();
        qreal pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, pts, &m_lifeCycleManager->freeFunc};
    }

    qreal viewFront() override {
        return frameQueue->viewFront<qreal>([&](std::vector<AVFrame *>* stk) {
            if (stk) {
                if (stk->empty()) {
                    throw std::runtime_error("reverse viewFront");
                }
                auto frame = stk->back();
                if (!frame)
                    return std::numeric_limits<qreal>::quiet_NaN();


                return static_cast<qreal>(frame->pts) * av_q2d(stream->time_base);
            }
            else {
                return std::numeric_limits<qreal>::quiet_NaN();
            }
        });
    }

    int skip(const std::function<bool(qreal)> &predicate) override {
        return 0;
    }
};

template<> class ReverseDecoderImpl<Audio>: public ReverseDecoderImpl<Common> {
    SwrContext *swrCtx = nullptr;
    uint8_t *audioOutBuf = nullptr;
    AVFrame * sampleFrameBuf = nullptr;
public:
    ReverseDecoderImpl(
            AVStream *vs,
            TwinsBlockQueue<std::vector<AVFrame*>*> *queue,
            LifeCycleManager *lifeCycleManager, ReverseDecoderImpl<Common>* follower
    ) : ReverseDecoderImpl<Common>(vs, queue, lifeCycleManager, follower) {
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

    virtual ~ReverseDecoderImpl() override {
        if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
        if (audioOutBuf) { av_freep(&audioOutBuf); }
        if (swrCtx) { swr_free(&swrCtx); }
    }

    void pushEOF() {
        std::cerr << "audio: pushEOF";
        frameStack->push_back(nullptr);
        frameQueue->push(frameStack);
        frameStack = nullptr;
    }

    void reverseSample(uint8_t *samples, int len) {
        len /= 2;
        auto reverse = reinterpret_cast<uint16_t*>(samples);
        int left = 0, right = len-1;
        while (left < right) {
            std::swap(reverse[left++], reverse[right--]);
        }
    }

    PONY_THREAD_SAFE AudioFrame getSample() override {
        //std::cerr << "audio get sample"<< std::endl;
        auto stk = frameQueue->viewFront<std::vector<AVFrame *>*>([](std::vector<AVFrame *>* stk){
            return stk;
        });
        if (!stk) return {};
        if (stk->empty()) {
            throw std::runtime_error("reverse getPicture");
        }
        auto frame = stk->back();
        stk->pop_back();
        if (stk->empty()) {
            delete stk;
            frameQueue->remove(true);
        }
        if (!frame) {
            qDebug() << "Reverse: get EOF Frame";
            return {};
        }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(nullptr, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        m_lifeCycleManager->pop();
        m_lifeCycleManager->freeFrame(frame);
        reverseSample(audioOutBuf, out_size);
        return {reinterpret_cast<std::byte *>(audioOutBuf), out_size, pts};
    }

};

