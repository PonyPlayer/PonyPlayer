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


class VirtualAudioDecoder : public IDemuxDecoder {
private:
    qreal m_videoDuration;
    PonyAudioFormat m_format = PonyPlayer::DEFAULT_AUDIO_FORMAT;
    std::byte *m_samples;
    std::mutex mutex;
    qreal nextPts{0.0};
public:
    explicit VirtualAudioDecoder(qreal videoDuration) :
        m_videoDuration(videoDuration), m_samples(new std::byte[4096]) {
        for (int i = 0; i < 4096; i++)
            m_samples[i] = static_cast<std::byte>(0);
    }

    ~VirtualAudioDecoder() {
        delete [] m_samples;
    }

    PONY_THREAD_SAFE void vaudioSeek(qreal pos) override{
        std::unique_lock lock(mutex);
        qDebug() << "nextPts: " << pos;
        nextPts = pos;
    }

    PONY_THREAD_SAFE bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        return !interrupt;
    }

    PONY_THREAD_SAFE void flushFFmpegBuffers() override {}

    PONY_THREAD_SAFE VideoFrameRef getPicture() override {NOT_IMPLEMENT_YET}

    PONY_THREAD_SAFE AudioFrame getSample() override {
        std::unique_lock lock(mutex);
        AudioFrame res{m_samples, 4096, nextPts};
        nextPts += PonyPlayer::DEFAULT_AUDIO_FORMAT.durationOfBytes(4096);
        return res;
    }

    PONY_THREAD_SAFE double duration() override {
        return m_videoDuration;
    }

    PONY_THREAD_SAFE qreal viewFront() override {
        std::unique_lock lock(mutex);
        return nextPts;
    }

    PONY_THREAD_SAFE int skip(const std::function<bool(qreal)> &predicate) override {
        return 0;
    }

    PONY_THREAD_SAFE void setEnable(bool b) override {}

    PONY_THREAD_SAFE PonyAudioFormat getInputFormat() override {
        return m_format;
    }

    PONY_THREAD_SAFE void setOutputFormat(const PonyAudioFormat &format) override {
        m_format = format;
    }

};