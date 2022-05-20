//
// Created by ColorsWind on 2022/5/18.
//

#pragma once

/**
 * 反向Decoder
 * @tparam type
 */
#include <iostream>
template<IDemuxDecoder::DecoderType type>
class ReverseDecoderImpl : public DecoderContext, public IDemuxDecoder {
protected:
    const qreal interval = 5.0;
    TwinsBlockQueue<AVFrame *> *frameQueue;
    std::vector<AVFrame*> *frameStack;
    ReverseDecoderImpl<Common>* follower{};
    qreal lastPts{-1.0};
    qreal from;
    qreal next{-1.0};

public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue, ReverseDecoderImpl<Common> *another) :
            DecoderContext(vs), frameQueue(queue), frameStack(new std::vector<AVFrame*>) {
        follower = another;
        from = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    void pushFrameStack() {
        while (!frameStack->empty()) {
            frameQueue->push(frameStack->back());
            frameStack->pop_back();
        }
    }

    double duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    qreal getLastPts() {
        return lastPts;
    }

    void clearFrameStack() {
        if (frameStack) {
            for (auto frame: *frameStack) {
                if (frame) av_frame_free(&frame);
            }
            frameStack->clear();
        }
    }

    ~ReverseDecoderImpl() override {
        clearFrameStack();
        delete frameStack;
    }

    void setStart(qreal secs) {
        from = secs;
        lastPts = -1.0;
        clearFrameStack();
    }

    qreal nextSegment() {
        auto res = next;
        next = -1.0;
        return res;
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
                lastPts = av_q2d(stream->time_base) * static_cast<double>(frameBuf->pts);
                if (lastPts < from-interval) {
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
                        if (from < interval)
                            from = 0;
                        else
                            from -= interval;
                        next = from;
                        return true;
                    }
                }
                else {
                    frameStack->push_back(frameBuf);
                    frameBuf = av_frame_alloc();
                }
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

    void flushFFmpegBuffers() override {
        avcodec_flush_buffers(codecCtx);
    }

    VideoFrameRef getPicture() override {
        NOT_IMPLEMENT_YET
    }

    AudioFrame getSample() override {
        NOT_IMPLEMENT_YET
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
template<>
class ReverseDecoderImpl<Video>: public ReverseDecoderImpl<Common> {
public:
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue, ReverseDecoderImpl<Common> *follower)
            : ReverseDecoderImpl<Common>(vs, queue, follower) {}

    void pushEOF() {
        qDebug() << "video push EOF";
        frameQueue->push(nullptr);
    }

    VideoFrameRef getPicture() override {
        AVFrame *frame = frameQueue->remove(true);
        if (!frame) {
            qDebug() << "getPicture: get EOF";
            return {};
        }
//        m_lifeCycleManager->pop();
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, true, pts};
    }

    qreal viewFront() override {
        return frameQueue->viewFront<qreal>([this](AVFrame * frame) {
            if (frame) {
                return static_cast<qreal>(frame->pts) * av_q2d(stream->time_base);
            } else {
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
    ReverseDecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *queue, ReverseDecoderImpl<Common> *follower)
            : ReverseDecoderImpl<Common>(vs, queue, follower) {
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

    ~ReverseDecoderImpl() override {
        if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
        if (audioOutBuf) { av_freep(&audioOutBuf); }
        if (swrCtx) { swr_free(&swrCtx); }
    }

    void pushEOF() {
        qDebug() << "audio: pushEOF";
        frameQueue->push(nullptr);
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
        auto *frame = std::forward<AVFrame *>(frameQueue->remove(true)); // suppress warning
        if (!frame) {
            qDebug() << "getSample: get EOF";
            return {};
        }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(nullptr, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        reverseSample(audioOutBuf, out_size);
        av_frame_free(&frame);
        return {reinterpret_cast<std::byte *>(audioOutBuf), out_size, pts};
    }

};

