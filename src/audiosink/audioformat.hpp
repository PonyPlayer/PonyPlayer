//
// Created by ColorsWind on 2022/5/15.
//
#pragma once

#include "helper.hpp"
#include <QtCore>
#include <utility>

#include <portaudio.h>


struct AudioDataInfo {
    qint32 origLength;
    qint32 processedLength;
    qreal speedUpRate;
};

struct PonySampleFormat {
private:
    using TransformFunc = std::function<void(std::byte*, qreal, unsigned long)>;

    int m_index;
    PaSampleFormat m_paSampleFormat;
    int m_bytesPerSample;
    std::function<void(std::byte*, qreal, unsigned long)> m_transform;


    PonySampleFormat(
        int mIndex,
        PaSampleFormat mPaSampleFormat,
        int mBytesPerSample,
        TransformFunc mTransform
    ) : m_index(mIndex), m_paSampleFormat(mPaSampleFormat), m_bytesPerSample(mBytesPerSample), m_transform(std::move(mTransform)) {}

public:
    template<class T>
    static PonySampleFormat of(PaSampleFormat paSamples) noexcept {
        static int id = 0;
        TransformFunc transform;
        size_t size;
        if constexpr(std::is_same<T, void>()) {
            transform = [](std::byte *src_, qreal factor, unsigned long samples){
                throw std::runtime_error("Unsupported samples format.");
            };
            size = 0xABCDEF;
        } else {
            transform = [](std::byte *src_, qreal factor, unsigned long samples){
                T *src = static_cast<T *>(static_cast<void *>(src_));
                for (size_t sampleOffset = 0; sampleOffset < samples; sampleOffset++) {
                    src[sampleOffset] = static_cast<T>(src[sampleOffset] * factor);
                }
            };
            size = sizeof(T);
        }
        return {id, paSamples, static_cast<int>(size), transform};
    }


    void transformSampleVolume(std::byte *src, qreal factor, unsigned long samples) const {
        m_transform(src, factor, samples);
    }

    bool operator==(const PonySampleFormat &rhs) const {
        return this->m_index == rhs.m_index;
    }
    bool operator!=(const PonySampleFormat &rhs) const {
        return !(rhs == *this);
    }

    [[nodiscard]] PaSampleFormat getPaSampleFormat() const {
        return m_paSampleFormat;
    }

    [[nodiscard]] int getBytesPerSample() const {
        return m_bytesPerSample;
    }

};
namespace PonyPlayer {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
    const PonySampleFormat Unknown =  PonySampleFormat::of<void>(0xABCDE);
    const PonySampleFormat UInt8   =  PonySampleFormat::of<uint8_t>(paUInt8);
    const PonySampleFormat Int16   =  PonySampleFormat::of<int16_t>(paInt16);
    const PonySampleFormat Int32   =  PonySampleFormat::of<int32_t>(paInt32);
    const PonySampleFormat Float   =  PonySampleFormat::of<float_t>(paFloat32);
#pragma GCC diagnostic pop
}


class PonyAudioFormat {
private:
    PonySampleFormat m_sampleFormat;
    int m_channelCount;
    int m_sampleRate;
public:

    PonyAudioFormat(
        PonySampleFormat sampleFormat,
        int sampleRate,
        int channelCount
    ) noexcept: m_sampleFormat(std::move(sampleFormat)), m_channelCount(channelCount), m_sampleRate(sampleRate) {}


    [[nodiscard]] const PonySampleFormat& getSampleFormat() const { return m_sampleFormat; }

    [[nodiscard]] PaSampleFormat getSampleFormatForPA() const {
        return m_sampleFormat.getPaSampleFormat();
    }

    [[nodiscard]] qreal durationOfBytes(int64_t bytes) const {
        return static_cast<qreal>(bytes) / (m_sampleRate * m_channelCount * getBytesPerSample());
    }

    [[nodiscard]] int64_t bytesOfDuration(qreal duration) const {
        return static_cast<int64_t>(duration * m_sampleRate * m_channelCount * getBytesPerSample());
    }

    [[nodiscard]] int getBytesPerSample() const {
        return m_sampleFormat.getBytesPerSample();
    }

    [[nodiscard]] int getBytesPerSampleChannels() const {
        return m_sampleFormat.getBytesPerSample() * m_channelCount;
    }

    [[nodiscard]] int getSampleRate() const {
        return m_sampleRate;
    }

    [[nodiscard]] int getChannelCount() const {
        return m_channelCount;
    }

    [[nodiscard]] int64_t suggestedRingBuffer(qreal speedFactor) const {
        return qBound<int64_t>(
            static_cast<int64_t>(2 * 1024 * m_channelCount * m_sampleFormat.getBytesPerSample()),
            bytesOfDuration(0.2 * speedFactor),
            256 << 20
        );
    }
};
