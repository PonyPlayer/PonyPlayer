//
// Created by ColorsWind on 2022/6/1.
//

#pragma once


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

    PONY_THREAD_SAFE PonyAudioFormat getInputFormat() override { NOT_IMPLEMENT_YET }

    PONY_THREAD_SAFE void setOutputFormat(const PonyAudioFormat &format) override { NOT_IMPLEMENT_YET }

};
