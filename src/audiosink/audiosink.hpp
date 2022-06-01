#pragma once

#include "portaudio.h"
#include "ponyplayer.h"
#include <utility>
#include <vector>
#include <QBuffer>
#include <QDebug>
#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "readerwriterqueue.h"
#include "sonic.h"
#include "audioformat.hpp"
#include "hotplug.h"
#include "helper.hpp"
#include <mutex>
#include <shared_mutex>

class HotPlugDetector;

enum class PlaybackState {
    PLAYING, ///< 正在播放
    STOPPED, ///< 停止状态
    PAUSED,  ///< 暂停状态
};

#define ASSERT_PA_OK(err, message) if ((err) != paNoError) { \
qWarning() << "Error" << Pa_GetErrorText(err); \
ILLEGAL_STATE(message); \
}

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

    PaStream *m_stream{};
    PaStreamParameters *param{};
    qreal m_volume, m_pitch;
    PlaybackState m_state;
    std::atomic<bool> m_pauseRequested = false; // 当播放完缓存的音频后停止
    HotPlugDetector *hotPlugDetector;
    QList<QString> devicesList;
    QString selectedOutputDevice;
    static std::mutex paStreamLock;
    static std::atomic_bool paInitialized;

    PonyAudioFormat m_format;
    PonyAudioFormat m_deviceFormat;
    size_t m_bufferMaxBytes;
    size_t m_sonicBufferMaxBytes;
    qreal m_speedFactor;
    PaUtilRingBuffer m_ringBuffer{};
    std::byte *m_ringBufferData;
    moodycamel::ReaderWriterQueue<AudioDataInfo> dataInfoQueue;

    sonicStream sonStream;
    std::byte *sonicBuffer = nullptr;

    PaTime m_startPoint = 0.0;
    std::atomic<int64_t> m_dataWritten = 0;
    std::atomic<int64_t> m_dataLastWrote = 0;


    std::atomic<bool> m_blockingState = false;
    std::mutex m_waitCompleteMutex;
    std::condition_variable m_waitCompleteCond;

    void m_paStreamFinishedCallback() {
        qDebug() << "Stream finished callback.";
        m_state = PlaybackState::PAUSED;
        if (m_state == PlaybackState::PLAYING) {
            emit resourceInsufficient();
        }
        m_waitCompleteCond.notify_all();
        emit stateChanged();

    }

    int m_paCallback(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags) {
        ring_buffer_size_t bytesAvailCount = PaUtil_GetRingBufferReadAvailable(&m_ringBuffer);
        auto bytesNeeded = static_cast<ring_buffer_size_t>(framesPerBuffer *
                                                           static_cast<unsigned long>(m_format.getBytesPerSampleChannels()));
        if (m_blockingState) {
            memset(outputBuffer, 0, static_cast<size_t>(bytesNeeded));
        } else if (bytesAvailCount == 0) {
            memset(outputBuffer, 0, static_cast<size_t>(bytesNeeded));
            if (m_pauseRequested) {
                return paComplete;
            } else {
                qWarning() << "paAbort bytesAvailCount == 0";
            }
        } else {
            ring_buffer_size_t timeAlignedByteWritten = 0; // 透明化加速的影响，表示在1x速度下，理应有多少个Byte被写入
            ring_buffer_size_t byteToBeWritten = std::min(bytesNeeded, bytesAvailCount); // 实际要往PortAudio的Buffer里写多少Byte
            ring_buffer_size_t byteRemainToAlign = byteToBeWritten; // 当前还需要处理多少个timeAlignedByteWritten
            while (byteRemainToAlign) {
                auto *audioDataInfo = dataInfoQueue.peek();
                if (audioDataInfo->processedLength < byteRemainToAlign) {
                    timeAlignedByteWritten += audioDataInfo->origLength;
                    byteRemainToAlign -= audioDataInfo->processedLength;
                    dataInfoQueue.pop();
                } else {
                    ring_buffer_size_t origLengthReduced = std::max(static_cast<ring_buffer_size_t>(1),
                                                                    static_cast<ring_buffer_size_t>(
                                                                            static_cast<double>(byteRemainToAlign) *
                                                                            audioDataInfo->speedUpRate));
                    audioDataInfo->processedLength -= static_cast<qint32>(byteRemainToAlign);
                    audioDataInfo->origLength -= static_cast<qint32>(origLengthReduced);
                    timeAlignedByteWritten += origLengthReduced;
                    byteRemainToAlign = 0;
                }
            }
            if (bytesNeeded > bytesAvailCount) {
                qWarning() << "paAbort bytesAvailCount < bytesNeeded";
                PaUtil_ReadRingBuffer(&m_ringBuffer, outputBuffer, bytesAvailCount);
                memset(static_cast<std::byte *>(outputBuffer) + byteToBeWritten, 0,
                       static_cast<size_t>(bytesNeeded - byteToBeWritten));
            } else {
                PaUtil_ReadRingBuffer(&m_ringBuffer, outputBuffer, byteToBeWritten);
            }
            m_dataWritten += timeAlignedByteWritten;
            m_dataLastWrote = timeAlignedByteWritten;
        }
        return paContinue;
    }

    PaError startStreamSafe() {
        PaError err = Pa_StopStream(m_stream);
        if (err != paStreamIsStopped && err != paNoError) {
            return err;
        }
        return Pa_StartStream(m_stream);
    }


    static void printError(PaError error) {
        qDebug() << "Error" << Pa_GetErrorText(error);
    }

    PaDeviceIndex getCurrentOutputDeviceIndex() {
        int deviceCount = Pa_GetDeviceCount();
        for (auto index = 0; index < deviceCount; index++) {
            auto *device = Pa_GetDeviceInfo(index);
            if (strcmp(device->name, selectedOutputDevice.toStdString().data()) == 0 && device->maxOutputChannels > 0) {
                return index;
            }
        }
        return Pa_GetDefaultOutputDevice();
    }

    // this should be guarded by paStreamLock
    void initializeStream() {
        if (!paInitialized) {
            Pa_Initialize();
            paInitialized = true;
            qDebug() << "Initialize PonyAudioSink backend.";
        }
        param = new PaStreamParameters;
        if (selectedOutputDevice.isNull()) {
            _getDeviceList();
            selectedOutputDevice = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->name;
        }
        param->device = getCurrentOutputDeviceIndex();
        selectedOutputDevice = Pa_GetDeviceInfo(param->device)->name;
        param->channelCount = m_format.getChannelCount();
        if (param->device == paNoDevice)
            throw std::runtime_error("no audio device!");
        param->channelCount = m_format.getChannelCount();
        param->sampleFormat = m_format.getSampleFormatForPA();
        param->suggestedLatency = Pa_GetDeviceInfo(param->device)->defaultLowOutputLatency;
        param->hostApiSpecificStreamInfo = nullptr;
        ASSERT_PA_OK(
                Pa_OpenStream(&m_stream, nullptr, param, m_format.getSampleRate(), paFramesPerBufferUnspecified,
                              paClipOff,
                              [](
                                      const void *inputBuffer,
                                      void *outputBuffer,
                                      unsigned long framesPerBuffer,
                                      const PaStreamCallbackTimeInfo *timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *userData
                              ) {
                                  return static_cast<PonyAudioSink *>(userData)->m_paCallback(inputBuffer, outputBuffer,
                                                                                              framesPerBuffer,
                                                                                              timeInfo, statusFlags);
                              }, this),
                "Can not open audio stream!"
        )
        const PaStreamInfo *info = Pa_GetStreamInfo(m_stream);
        m_deviceFormat = PonyAudioFormat(PonyPlayer::valueOf(paInt16), info->sampleRate,
                                         param->channelCount);
        ASSERT_PA_OK(Pa_SetStreamFinishedCallback(m_stream, [](void *userData) {
            static_cast<PonyAudioSink *>(userData)->m_paStreamFinishedCallback();
        }), "Can not set stream callback!")

    }

    QString stateToStr() {
        switch (m_state) {
            case PlaybackState::PLAYING:
                return "PLAYING";
            case PlaybackState::PAUSED:
                return "PAUSED";
            case PlaybackState::STOPPED:
                return "STOPPED";
        }
        return "UNKNOWN";
    }


public:
    constexpr const static qreal MAX_SPEED_FACTOR = 4;

    /**
     * 创建PonyAudioSink并attach到默认设备上
     * @param format 音频格式
     * @param bufferSizeAdvice DataBuffer 的建议大小, PonyAudioSink 保证实际的 DataBuffer 不小于建议大小.
     */
    PonyAudioSink(PonyAudioFormat format) : m_volume(0.5), m_pitch(1.0), m_state(PlaybackState::STOPPED),
                                            m_format(std::move(format)),
                                            m_speedFactor(1.0), m_deviceFormat(PonyPlayer::Unknown, 0, 0) {


        paStreamLock.lock();
        initializeStream();
        paStreamLock.unlock();
        m_bufferMaxBytes = nextPowerOf2(static_cast<unsigned>(m_format.suggestedRingBuffer(MAX_SPEED_FACTOR)));
        m_sonicBufferMaxBytes = m_bufferMaxBytes * 4;
        m_ringBufferData = static_cast<std::byte *>(PaUtil_AllocateMemory(static_cast<long>(m_bufferMaxBytes)));
        if (PaUtil_InitializeRingBuffer(&m_ringBuffer,
                                        sizeof(std::byte),
                                        static_cast<ring_buffer_size_t>(m_bufferMaxBytes),
                                        m_ringBufferData) < 0)
            throw std::runtime_error("can not initialize ring buffer!");
        sonicBuffer = new std::byte[m_sonicBufferMaxBytes];
        sonStream = sonicCreateStream(m_format.getSampleRate(), m_format.getChannelCount());
        sonicSetChordPitch(sonStream, 1);
        sonicSetSpeed(sonStream, static_cast<float>(m_speedFactor));
        // HotPlugDetector should be created after PA stream open
        hotPlugDetector = new HotPlugDetector(this);
    }

    /**
     * 析构即从deattach当前设备
     */
    ~PonyAudioSink() override {
        std::lock_guard lock(paStreamLock);
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
        std::lock_guard lock(paStreamLock);
        m_pauseRequested = false;
        qDebug() << "Audio start.";
        if (m_state == PlaybackState::PLAYING) {
            qDebug() << "AudioSink already started.";
            return;
        }
        PaError err = startStreamSafe();
        if (err != paNoError) {
            qWarning() << "Error at starting stream:" << Pa_GetErrorText(err);
            ILLEGAL_STATE("Can not start stream!.");
        }
        m_state = PlaybackState::PLAYING;
        qDebug() << "Pa stream started";

        emit stateChanged();
    }

    /**
     * 暂停播放, 状态变为 PlaybackState::PAUSED. 但已经写入AudioBuffer的音频将会继续播放.
     */
    void pause() {
        std::lock_guard lock(paStreamLock);
        qDebug() << "Audio requesting pause. Current state is " << stateToStr();
        if (m_state == PlaybackState::PLAYING) {
            Pa_StopStream(m_stream);
            qDebug() << "Stream Stopped";
            m_state = PlaybackState::PAUSED;
        } else if (m_state == PlaybackState::STOPPED) {
            // ignore
        } else {
            qWarning() << "AudioSink already paused.";
        }
    }

    void waitComplete() {
        m_pauseRequested = true;
        std::unique_lock lock(m_waitCompleteMutex);
        m_waitCompleteCond.wait(lock);
    }

    /**
     * 停止播放, 状态变为 PlaybackState::STOPPED, 且已写入AudioBuffer的音频将会被放弃, 播放会立即停止.
     */
    void stop() {
        std::lock_guard lock(paStreamLock);
        qDebug() << "Audio stateStop.";
        if (m_state == PlaybackState::PLAYING || m_state == PlaybackState::PAUSED) {
            Pa_AbortStream(m_stream);
            m_state = PlaybackState::STOPPED;
        } else {
            qWarning() << "AudioSink already stopped.";
        }
    }

    void setBlockState(bool state) {
        m_blockingState = state;
    }

    bool isBlock() {
        return m_blockingState;
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
    [[nodiscard]] int64_t freeByte() const {
        return static_cast<int64_t>(PaUtil_GetRingBufferWriteAvailable(&m_ringBuffer))
               - static_cast<int64_t>((MAX_SPEED_FACTOR - m_speedFactor) * static_cast<qreal>(m_bufferMaxBytes) /
                                      MAX_SPEED_FACTOR);
    }

    /**
     * 写AudioBuffer, 要么写入完全成功, 要么失败. 这个操作保证在VideoThread上进行.
     * @param buf 数据源
     * @param origLen 长度(单位: byte)
     * @return 写入是否成功
     */
    bool write(const char *buf, qint32 origLen) {
        int len = 0;
        if (m_format.getSampleFormat() != PonyPlayer::Int16) {
            throw std::runtime_error("Only support Int16!");
        }
        if (origLen % m_format.getBytesPerSampleChannels() == 0) {
            sonicWriteShortToStream(sonStream, reinterpret_cast<const short *>(buf),
                                    static_cast<int>(origLen) / m_format.getBytesPerSampleChannels());
            int currentLen = 0;
            while ((currentLen = sonicReadShortFromStream(sonStream,
                                                          reinterpret_cast<short *>
                                                          (sonicBuffer + len * m_format.getChannelCount()),
                                                          0x7fffffff))) {
                len += currentLen;
            }
        } else
            ILLEGAL_STATE("Incomplete Int16!");
        len *= m_format.getBytesPerSampleChannels();
        ring_buffer_size_t bufAvailCount = PaUtil_GetRingBufferWriteAvailable(&m_ringBuffer);

        if (bufAvailCount < len) return false;
        void *ptr[2] = {nullptr};
        ring_buffer_size_t sizes[2] = {0};

        PaUtil_GetRingBufferWriteRegions(&m_ringBuffer, static_cast<ring_buffer_size_t>(len), &ptr[0], &sizes[0],
                                         &ptr[1],
                                         &sizes[1]);
        memcpy(ptr[0], sonicBuffer, static_cast<size_t>(sizes[0]));
        memcpy(ptr[1], sonicBuffer + sizes[0], static_cast<size_t>(sizes[1]));
        dataInfoQueue.enqueue({origLen, len, static_cast<qreal>(origLen) / len});
        PaUtil_AdvanceRingBufferWriteIndex(&m_ringBuffer, static_cast<ring_buffer_size_t>(len));
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
        PaUtil_FlushRingBuffer(&m_ringBuffer);
        return 0;
    }


    /**
     * 获取当前播放的时间, 这个函数只能在 PlaybackState::PLAYING 或 PlaybackState::PAUSED 状态下使用.
     * @return 当前已播放音频的长度(单位: 秒)
     */
    [[nodiscard]] qreal getProcessSecs(bool backward) const {
        if (m_state == PlaybackState::STOPPED) { return m_startPoint; }
        auto processSec = static_cast<double>(m_format.durationOfBytes(m_dataWritten - m_dataLastWrote));
        if (backward) {
            return m_startPoint - processSec;
        } else {
            return m_startPoint + processSec;
        }
    }

    /**
     * 设置下一次播放的计时器. 这个函数必须在 PlaybackState::STOPPED 状态下使用. 在播放开始后, 设置生效。
     * @param t 新的播放时间(单位: 秒)
     */
    void setStartPoint(double t = 0.0) {
        if (std::isnan(t)) {
            qWarning() << "Trying set start point to NaN";
        }
        if (m_state == PlaybackState::STOPPED) {
            m_startPoint = t;
            m_dataWritten = 0;
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
     * 设置音调
     * @param newPitch
     */
    void setPitch(qreal newPitch) {
        m_pitch = qBound(0.0, newPitch, 16.0);
        sonicSetPitch(sonStream, static_cast<float>(newPitch));
    }

    /**
     * 设置速度
     * @param newSpeed
     */
    void setSpeed(qreal newSpeed) {
        m_speedFactor = qBound(0.0, newSpeed, MAX_SPEED_FACTOR);
        sonicSetSpeed(sonStream, static_cast<float>(newSpeed));
    }

    /**
     * 获取当前音量
     * @return
     */
    [[nodiscard]] qreal volume() const {
        return m_volume;
    }

    [[nodiscard]] qreal speed() const {
        return m_speedFactor;
    }

    [[nodiscard]] qreal pitch() const {
        return m_pitch;
    }

    void _getDeviceList() {
        devicesList.clear();
        int devicesCount = Pa_GetDeviceCount();
        for (auto index = 0; index < devicesCount; index++) {
            auto deviceInfo = Pa_GetDeviceInfo(index);
            QString deviceName = Pa_GetDeviceInfo(index)->name;
            if (deviceInfo->maxOutputChannels < 2) continue;
#ifdef WIN32
            if (deviceInfo->hostApi != PaHostApiTypeId::paDirectSound) continue;
#endif
            devicesList.push_back(deviceName);
            qDebug() << deviceName;
        }
    }

    QString getSelectedOutputDevice() { return selectedOutputDevice; }

    void restartStream(const std::function<void()> &betweenInitAndOpen) {
        std::lock_guard lock(paStreamLock);
        if (paInitialized) {
            Pa_AbortStream(m_stream);
            Pa_Terminate();
        }
        Pa_Initialize();
        paInitialized = true;
        if (betweenInitAndOpen) betweenInitAndOpen();
        initializeStream();
        if (m_state == PlaybackState::PLAYING) {
            startStreamSafe();
        }
    }

    void refreshDevicesList() {
        qDebug() << "Refreshing Devices list...";
        auto middleFunc = [this] {
            _getDeviceList();
        };
        restartStream(middleFunc);
        emit signalAudioOutputDeviceListChanged();
        emit signalDeviceSwitched();
    }

    QStringList getAudioDeviceList() {
        return devicesList;
    }

    void setFormat(const PonyAudioFormat &format) {
        std::unique_lock lock(paStreamLock);
        m_format = {PonyPlayer::Int16, format.getSampleRate(), format.getChannelCount()};
        lock.unlock();
        restartStream(nullptr);
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

    void signalAudioOutputDeviceListChanged();

    void signalDeviceSwitched();

public slots:

    void audioOutputDevicesChanged() {
        refreshDevicesList();
    }

    void requestDeviceSwitch(const QString &device) {
        qDebug() << "change audio output device to " << device;
        selectedOutputDevice = device;
        std::unique_lock lock(paStreamLock);
        if (paInitialized) {
            Pa_AbortStream(m_stream);
            Pa_Terminate();
        }
        paInitialized = false;
        initializeStream();
        if (m_state == PlaybackState::PLAYING) {
            startStreamSafe();
        }
        lock.unlock();
        emit signalDeviceSwitched();
    }

    PonyAudioFormat getCurrentDeviceFormat() {
        return m_deviceFormat;
    }
};
