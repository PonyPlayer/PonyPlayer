#pragma once

#include "portaudio.h"
#include <vector>
#include <QBuffer>
#include <QDebug>
#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "readerwriterqueue.h"
#include "helper.hpp"
#include "sonic.h"
#include "audiofomrat.hpp"


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
    size_t m_bufferMaxBytes;
    size_t m_sonicBufferMaxBytes;
    size_t m_bytesPerSample;
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


    void m_paStreamFinishedCallback() {
        if (m_state == PlaybackState::PLAYING) {
            emit resourceInsufficient();
        }
        m_state = PlaybackState::PAUSED;
        emit stateChanged();
        qDebug() << "Stream finished callback.";
    }

    int m_paCallback(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags) {
        ring_buffer_size_t bytesAvailCount = PaUtil_GetRingBufferReadAvailable(&ringBuffer);
        auto bytesNeeded = static_cast<ring_buffer_size_t>(framesPerBuffer *
                                                           static_cast<unsigned long>(m_format.getChannelCount()) *
                                                           m_bytesPerSample);
        if (bytesAvailCount == 0) {
            memset(outputBuffer, 0, static_cast<size_t>(bytesNeeded));
            qWarning() << "paAbort bytesAvailCount == 0";
        } else {
            qint64 timeAlignedByteWritten = 0; // 透明化加速的影响，表示在1x速度下，理应有多少个Byte被写入
            qint64 byteToBeWritten = std::min(bytesNeeded, bytesAvailCount); // 实际要往PortAudio的Buffer里写多少Byte
            qint64 byteRemainToAlign = byteToBeWritten; // 当前还需要处理多少个timeAlignedByteWritten
            while (byteRemainToAlign) {
                auto *audioDataInfo = dataInfoQueue.peek();
                if (audioDataInfo->processedLength < byteRemainToAlign) {
                    timeAlignedByteWritten += audioDataInfo->origLength;
                    byteRemainToAlign -= audioDataInfo->processedLength;
                    dataInfoQueue.pop();
                } else {
                    qint64 origLengthReduced = std::max(1ll,
                                                        static_cast<qint64>(static_cast<double>(byteRemainToAlign) *
                                                                            audioDataInfo->speedUpRate));
                    audioDataInfo->processedLength -= byteRemainToAlign;
                    audioDataInfo->origLength -= origLengthReduced;
                    timeAlignedByteWritten += origLengthReduced;
                    byteRemainToAlign = 0;
                }
            }
            if (bytesNeeded > bytesAvailCount) {
                qWarning() << "paAbort bytesAvailCount < bytesNeeded";
                PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, bytesAvailCount);
                memset(static_cast<std::byte *>(outputBuffer) + byteToBeWritten, 0,
                       static_cast<size_t>(bytesNeeded - byteToBeWritten));
            } else {
                PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, byteToBeWritten);
            }
            dataWritten += timeAlignedByteWritten;
            dataLastWrote = timeAlignedByteWritten;
        }
        return paContinue;
    }


    void printError(PaError error) {
        qDebug() << "Error" << Pa_GetErrorText(error);
    }

public:

    /**
     * 创建PonyAudioSink并attach到默认设备上
     * @param format 音频格式
     * @param bufferSizeAdvice DataBuffer 的建议大小, PonyAudioSink 保证实际的 DataBuffer 不小于建议大小.
     */
    PonyAudioSink(PonyAudioFormat format, unsigned long bufferSizeAdvice) : m_format(format), m_speedFactor(1),
                                                                            m_volume(0.5),
                                                                            m_state(PlaybackState::STOPPED) {
        // initialize
        static bool initialized = false;
        if (!initialized) {
            Pa_Initialize();
            initialized = true;
            qDebug() << "Initialize PonyAudioSink backend.";
        }
        param = new PaStreamParameters;
        param->device = Pa_GetDefaultOutputDevice();
        param->channelCount = format.getChannelCount();
        if (param->device == paNoDevice)
            throw std::runtime_error("no audio device!");
        m_bytesPerSample = format.getBytesPerSample();
        m_bytesPerSample = format.getBytesPerSample();
        param->channelCount = format.getChannelCount();
        param->sampleFormat = format.getSampleFormatForPA();
        param->suggestedLatency = Pa_GetDeviceInfo(param->device)->defaultLowOutputLatency;
        param->hostApiSpecificStreamInfo = nullptr;
        PaError err = Pa_OpenStream(
                &m_stream,
                nullptr,
                param,
                m_format.getSampleRate(),
                paFramesPerBufferUnspecified,
                paClipOff,
                [](const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData) {
                    return static_cast<PonyAudioSink *>(userData)->m_paCallback(inputBuffer, outputBuffer,
                                                                                framesPerBuffer,
                                                                                timeInfo, statusFlags);
                },
                this
        );
        if (err != paNoError) {
            printError(err);
            throw std::runtime_error("can not open audio stream!");
        }
        m_bufferMaxBytes = nextPowerOf2(bufferSizeAdvice * m_format.getChannelCount());
        m_sonicBufferMaxBytes = m_bufferMaxBytes * 4;
        ringBufferData = PaUtil_AllocateMemory(static_cast<long>(m_bufferMaxBytes));
        if (PaUtil_InitializeRingBuffer(&ringBuffer,
                                        sizeof(std::byte),
                                        static_cast<ring_buffer_size_t>(m_bufferMaxBytes),
                                        ringBufferData) < 0)
            throw std::runtime_error("can not initialize ring buffer!");
        Pa_SetStreamFinishedCallback(&m_stream, [](void *userData) {
            static_cast<PonyAudioSink *>(userData)->m_paStreamFinishedCallback();
        });
        sonicBuffer = new char[m_sonicBufferMaxBytes];
        sonStream = sonicCreateStream(m_format.getSampleRate(), m_format.getChannelCount());
        sonicSetSpeed(sonStream, m_speedFactor);
    }

    /**
     * 析构即从deattach当前设备
     */
    ~PonyAudioSink() override {
        m_state = PlaybackState::STOPPED;
        PaError err = Pa_CloseStream(m_stream);
        m_stream = nullptr;
        if (err != paNoError) {
            qWarning() << "Error at Destroying PonyAudioSink" << Pa_GetErrorText(err);
        }
    }

    /**
     * 开始播放, 状态变为 PlaybackState::PLAYING. 若当前DataBuffer内容不足, 状态将会发生改变.
     * @see PonyAudioSink::stateChanged
     * @see PonyAudioSink::resourceInsufficient
     */
    void start() {
        qDebug() << "Audio start.";
        if (m_state == PlaybackState::PLAYING) {
            qDebug() << "AudioSink already started.";
            return;
        }
        PaError err = Pa_StartStream(m_stream);
        if (err != paNoError) {
            qWarning() << "Error at starting stream" << Pa_GetErrorText(err);
            throw std::runtime_error("Can not start stream!");
        }
        m_state = PlaybackState::PLAYING;

        emit stateChanged();
    }

    /**
     * 暂停播放, 状态变为 PlaybackState::PAUSED. 但已经写入AudioBuffer的音频将会继续播放.
     */
    void pause() {
        qDebug() << "Audio statePause.";
        if (m_state == PlaybackState::PLAYING) {
            Pa_StopStream(m_stream);
            m_state = PlaybackState::PAUSED;
        } else if (m_state == PlaybackState::STOPPED) {
            // ignore
        } else {
            throw std::runtime_error("AudioSink already paused.");
        }
    }

    /**
     * 停止播放, 状态变为 PlaybackState::STOPPED, 且已写入AudioBuffer的音频将会被放弃, 播放会立即停止.
     */
    void stop() {
        qDebug() << "Audio stateStop.";
        if (m_state == PlaybackState::PLAYING || m_state == PlaybackState::PAUSED) {
            Pa_AbortStream(m_stream);
            m_state = PlaybackState::STOPPED;
        } else {
            qWarning() << "AudioSink already stopped.";
        }
    }

    /**
     * 获取播放状态
     * @return 当前状态
     */
    [[nodiscard]] PlaybackState state() const {
        return m_state;
    }

    /**
     * 获取AudioBuffer剩余空间
     * @return 剩余空间(单位: byte)
     */
    [[nodiscard]] size_t freeByte() const {
        return static_cast<size_t>(PaUtil_GetRingBufferWriteAvailable(&ringBuffer));
    }

    /**
     * 写AudioBuffer, 要么写入完全成功, 要么失败. 这个操作保证在VideoThread上进行.
     * @param buf 数据源
     * @param origLen 长度(单位: byte)
     * @return 写入是否成功
     */
    bool write(const char *buf, qint64 origLen) {
        int len = 0;
        int sonicBufferOffset = 0;
        if (m_format.getSampleFormat() != PonyPlayer::Int16) {
            throw std::runtime_error("Only support Int16!");
        }
        if (origLen % (m_format.getChannelCount() * m_bytesPerSample) == 0) {
            sonicWriteShortToStream(sonStream, reinterpret_cast<const short *>(buf),
                                    static_cast<int>(origLen) / (m_format.getChannelCount() * m_bytesPerSample));
            int currentLen = 0;
            while ((currentLen = sonicReadShortFromStream(sonStream,
                                                          reinterpret_cast<short *>
                                                          (sonicBuffer + len * m_format.getChannelCount()),
                                                          0x7fffffff))) {
                len += currentLen;
            }
        } else throw std::runtime_error("Incomplete Int16!");
        len *= m_format.getChannelCount() * m_bytesPerSample;
        ring_buffer_size_t bufAvailCount = PaUtil_GetRingBufferWriteAvailable(&ringBuffer);

        if (bufAvailCount < len) return false;
        void *ptr[2] = {nullptr};
        ring_buffer_size_t sizes[2] = {0};

        PaUtil_GetRingBufferWriteRegions(&ringBuffer, static_cast<ring_buffer_size_t>(len), &ptr[0], &sizes[0], &ptr[1],
                                         &sizes[1]);
        memcpy(ptr[0], sonicBuffer, static_cast<size_t>(sizes[0]));
        memcpy(ptr[1], sonicBuffer + sizes[0], static_cast<size_t>(sizes[1]));
        dataInfoQueue.enqueue({origLen, len, static_cast<qreal>(origLen) / len});
        PaUtil_AdvanceRingBufferWriteIndex(&ringBuffer, static_cast<ring_buffer_size_t>(len));
        return true;
    }

    /**
     * 清空AudioBuffer, 将所有空间标记为可用. 这个操作保证在VideoThread上进行.
     * @return 清空数据长度(单位: byte)
     */
    size_t clear() {
        if (m_state != PlaybackState::STOPPED) {
            qWarning() << "clear make no effect when state != STOPPED.";
        }
        // 需要保证此刻没有读写操作
        PaUtil_FlushRingBuffer(&ringBuffer);
        return 0;
    }


    /**
     * 获取当前播放的时间, 这个函数只能在 PlaybackState::PLAYING 或 PlaybackState::PAUSED 状态下使用.
     * @return 当前已播放音频的长度(单位: 秒)
     */
    [[nodiscard]] qreal getProcessSecs(bool backward) const {
        if (m_state == PlaybackState::STOPPED) { return timeBase; }
        auto processSec = static_cast<double>(m_format.durationOfBytes(dataWritten - dataLastWrote));
        if (backward) {
            return timeBase - processSec;
        } else {
            return timeBase + processSec;
        }
    }

    /**
     * 设置下一次播放的计时器. 这个函数必须在 PlaybackState::STOPPED 状态下使用. 在播放开始后, 设置生效。
     * @param t 新的播放时间(单位: 秒)
     */
    void setStartPoint(double t = 0.0) {
        if (m_state == PlaybackState::STOPPED) {
            timeBase = t;
            dataWritten = 0;
            dataLastWrote = 0;
        } else {
            qWarning() << "setTimeBase make no effect when state != STOPPED";
        }
    }


    /**
     * 设备音量, 音量的范围通常是[0, 1]
     * @param newVolume
     */
    void setVolume(qreal newVolume) {
        m_volume = qBound(0.0, newVolume, 1.0);
        sonicSetVolume(sonStream, static_cast<float>(newVolume));
    }

    /**
     * 设置速度
     * @param newSpeed
     */
    void setSpeed(qreal newSpeed) {
        m_speedFactor = newSpeed;
        sonicSetSpeed(sonStream, newSpeed);
    }

    /**
     * 获取当前音量
     * @return
     */
    [[nodiscard]] qreal volume() const {
        return m_volume;
    }

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
