//
// Created by ColorsWind on 2022/5/1.
//
#pragma once

#include "decoders.hpp"

template<IDemuxDecoder::DecoderType type>
class DecoderImpl : public DecoderContext, public IDemuxDecoder {
protected:
    TwinsBlockQueue<AVFrame *> *frameQueue;
public:
    DecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue)
            : DecoderContext(vs), frameQueue(queue) {}

    PONY_THREAD_SAFE double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    PONY_GUARD_BY(DECODER) bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        if (!frameQueue->isEnable())
            return true;
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0 && !interrupt) {
            ret = avcodec_receive_frame(codecCtx, frameBuf);
            if (ret >= 0) {
                if(!frameQueue->push(frameBuf)) {
                    frameQueue->clear([](AVFrame *frame) { av_frame_free(&frame); });
                    av_frame_unref(frameBuf);
                    return false;
                }
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

    PONY_THREAD_SAFE VideoFrameRef getPicture() override { NOT_IMPLEMENT_YET }

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
        }, [](AVFrame *frame) { av_frame_free(&frame); });
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
    DecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue) : DecoderImpl<Common>(vs, queue) {
        AVChannelLayout layout;
        av_channel_layout_default(&layout, 2);
        /*
        this->swrCtx = swr_alloc_set_opts(swrCtx, av_get_default_channel_layout(2),
                                          AV_SAMPLE_FMT_S16, 44100,
                                          static_cast<int64_t>(codecCtx->channel_layout), codecCtx->sample_fmt,
                                          codecCtx->sample_rate, 0, nullptr);*/

        swr_alloc_set_opts2(&swrCtx, &layout,
                            AV_SAMPLE_FMT_S16, 44100,
                            &codecCtx->ch_layout, codecCtx->sample_fmt,
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
        if (!frameQueue->isEnable())
            throw std::runtime_error("forward: getSample is disabled");

        AVFrame *frame = frameQueue->remove(true);
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(nullptr, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        av_frame_free(&frame);
        return {reinterpret_cast<std::byte *>(audioOutBuf), out_size, pts};
    }

};

/**
 * 视频解码器实现
 */
template<> class DecoderImpl<Video>: public DecoderImpl<Common> {
private:
    /**
     * 如果视频的第一帧 pts < 0, 则说明第一帧为封面. 保存下来.
     */
    std::atomic<AVFrame *> stillVideoFrame = nullptr;
public:
    DecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue) : DecoderImpl<Common>(vs, queue) {}


    VideoFrameRef getPicture() override {
        if (stillVideoFrame != nullptr) { return {stillVideoFrame, true, -1}; }
        AVFrame *frame = frameQueue->remove(true);
        if (!frame) { return {}; }
        if (frame->pts < 0) {
            stillVideoFrame = frame;
            return {frame, true, -9};
        } else {double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
            return {frame, true, pts};
        }
    }


    ~DecoderImpl() override {
        auto *frame = stillVideoFrame.load();
        if (frame) { av_frame_free(&frame); }
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

    PONY_THREAD_SAFE VideoFrameRef getPicture() override {
        return {nullptr, true, std::numeric_limits<qreal>::quiet_NaN()};
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
