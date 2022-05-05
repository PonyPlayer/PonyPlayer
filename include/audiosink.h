#pragma once

#include <portaudio.h>
#include <vector>
#include <QtMultimedia/QtMultimedia>
#include <QBuffer>
#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "readerwriterqueue.h"
#include "helper.h"
#include "sonic.h"

struct AudioDataInfo {
    qint64 origLength;
    qint64 processedLength;
    qreal speedUpRate;
};

enum class PonySampleFormat {
    Unknown,
    UInt8,
    Int16,
    Int32,
    Float,
    NSampleFormats
};

class PonyAudioFormat {
private:
    PonySampleFormat sampleFormat;
    int channelCount{};
    int sampleRate{};
    PaSampleFormat paSampleFormat{};
    size_t bytesPerSample{};
public:
    PonyAudioFormat() = default;

    PonyAudioFormat(PonySampleFormat sampleFormat_, int channelCount_, int sampleRate_);

    void setSampleFormat(PonySampleFormat sampleFormat_);

    void setChannelCount(int channelCount_) { channelCount = channelCount_; }

    void setSampleRate(int sampleRate_) { sampleRate = sampleRate_; }

    PonySampleFormat getSampleFormat() const { return sampleFormat; }

    PaSampleFormat getSampleFormatForPA() const;

    int getChannelCount() const { return channelCount; }

    int getSampleRate() const { return sampleRate; }

    size_t getBytesPerSample() const;
};


enum class PlaybackState {
    PLAYING, ///< 正在播放
    STOPPED, ///< 停止状态
    PAUSED,  ///< 暂停状态
};

/**
 * @brief 播放音频裸流, 用于代替QAudioSink.
 *
 * 这个类的函数都不是线程安全的, 必须保证在 VideoThread 中调用, 这个类的RAII的. 音频播放涉及两个缓存: AudioBuffer
 * 和 DataBuffer. AudioBuffer 由系统维护, 一旦我们向里面写入数据, 我们将不能读取它. 音频播放时, 系统播放 AudioBuffer
 * 中的音频. 当 AudioBuffer 数据不足时, 系统会通过回调函数从 DataBuffer 中获取数据. DataBuffer 由 PonyAudioSink
 * 维护, 当需要播放音频时需要先调用 write 函数将音频数据写入 DataBuffer.
 */
class PonyAudioSink : public QObject {
Q_OBJECT
private:
    PaStream *m_stream;
    PaStreamParameters *param;
    PaTime timeBase;
    PonyAudioFormat m_format;
    int m_sampleRate;
    PonySampleFormat m_sampleFormat;
    size_t m_bufferMaxBytes;
    size_t m_sonicBufferMaxBytes;
    size_t m_bytesPerSample;
    int m_channelCount;
    qreal m_speedFactor;
    void *ringBufferData;
    qreal m_volume;
    std::atomic<int64_t> dataWritten = 0;
    std::atomic<int64_t> dataLastWrote = 0;
    PlaybackState m_state;
    PaUtilRingBuffer ringBuffer;
    moodycamel::ReaderWriterQueue<AudioDataInfo> dataInfoQueue;
    sonicStream sonStream;
    char *sonicBuffer = nullptr;


    void m_paStreamFinishedCallback();

    int m_paCallback(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags);

    void transformVolume(void *buffer, unsigned long framesPerBuffer) const;

    template<typename T>
    void transformSampleVolume(std::byte *src_, qreal factor, unsigned long samples) const;


public:
    /**
     * 计算音频长度对应的数据大小
     * @param format 音频格式
     * @param duration 音频时长(单位: us)
     * @return 数据大小(单位: byte)
     */
    inline static int durationToSize(const QAudioFormat &format, int64_t duration) {
        return format.bytesForDuration(duration);
    }


    /**
     * 计算音频数据长度对应的数据大小
     * @param format 音频格式
     * @param bytes 数据大小(单位: byte)
     * @return 音频时长(单位: us)
     */
    inline static int64_t sizeToDuration(const QAudioFormat &format, int bytes) {
        return format.durationForBytes(bytes);
    }

    /**
     * 创建PonyAudioSink并attach到默认设备上
     * @param format 音频格式
     * @param bufferSizeAdvice DataBuffer 的建议大小, PonyAudioSink 保证实际的 DataBuffer 不小于建议大小.
     */
    PonyAudioSink(PonyAudioFormat format, unsigned long bufferSizeAdvice);

    /**
     * 析构即从deattach当前设备
     */
    ~PonyAudioSink() override;

    /**
     * 开始播放, 状态变为 PlaybackState::PLAYING. 若当前DataBuffer内容不足, 状态将会发生改变.
     * @see PonyAudioSink::stateChanged
     * @see PonyAudioSink::resourceInsufficient
     */
    void start();

    /**
     * 暂停播放, 状态变为 PlaybackState::PAUSED. 但已经写入AudioBuffer的音频将会继续播放.
     */
    void pause();

    /**
     * 停止播放, 状态变为 PlaybackState::STOPPED, 且已写入AudioBuffer的音频将会被放弃, 播放会立即停止.
     */
    void stop();

    /**
     * 获取播放状态
     * @return 当前状态
     */
    [[nodiscard]] PlaybackState state() const;

    /**
     * 获取AudioBuffer剩余空间
     * @return 剩余空间(单位: byte)
     */
    [[nodiscard]] size_t freeByte() const;

    /**
     * 写AudioBuffer, 要么写入完全成功, 要么失败. 这个操作保证在VideoThread上进行.
     * @param buf 数据源
     * @param origLen 长度(单位: byte)
     * @return 写入是否成功
     */
    bool write(const char *buf, qint64 origLen);

    /**
     * 清空AudioBuffer, 将所有空间标记为可用. 这个操作保证在VideoThread上进行.
     * @return 清空数据长度(单位: byte)
     */
    size_t clear();


    /**
     * 获取当前播放的时间, 这个函数只能在 PlaybackState::PLAYING 或 PlaybackState::PAUSED 状态下使用.
     * @return 当前已播放音频的长度(单位: 秒)
     */
    [[nodiscard]] qreal getProcessSecs() const;

    /**
     * 设置下一次播放的计时器. 这个函数必须在 PlaybackState::STOPPED 状态下使用. 在播放开始后, 设置生效。
     * @param t 新的播放时间(单位: 秒)
     */
    void setStartPoint(double t = 0.0);


    /**
     * 设备音量, 音量的范围通常是[0, 1]
     * @param newVolume
     */
    void setVolume(qreal newVolume);

    /**
     * 获取当前音量
     * @return
     */
    [[nodiscard]] qreal volume() const;

signals:

    /**
     * 播放状态发生改变
     */
    void stateChanged();

    /**
     * 由于缺少音频数据, 被迫暂停播放
     */
    void resourceInsufficient();

};
