#include "audiosink.h"
#include "helper.h"
#include "sonic.h"

PonyAudioSink::PonyAudioSink(PonyAudioFormat format, unsigned long bufferSizeAdvice)
        : m_stream(nullptr), timeBase(0), m_volume(1.0), m_speedFactor(2), m_state(PlaybackState::STOPPED) {
    // initialize
    static bool initialized = false;
    if (!initialized) {
        Pa_Initialize();
        initialized = true;
        qDebug() << "Initialize PonyAudioSink backend.";
    }
    m_format = format;
    param = new PaStreamParameters;
    param->device = Pa_GetDefaultOutputDevice();
    if (param->device == paNoDevice)
        throw std::runtime_error("no audio device!");
    m_channelCount = format.getChannelCount();
    m_bytesPerSample = format.getBytesPerSample();
    m_sampleFormat = format.getSampleFormat();
    param->channelCount = m_channelCount;
    param->sampleFormat = format.getSampleFormatForPA();
    param->suggestedLatency = Pa_GetDeviceInfo(param->device)->defaultLowOutputLatency;
    param->hostApiSpecificStreamInfo = nullptr;
    m_sampleRate = format.getSampleRate();
    PaError err = Pa_OpenStream(
            &m_stream,
            nullptr,
            param,
            m_sampleRate,
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
    if (err != paNoError)
        throw std::runtime_error("can not open audio stream!");
    m_bufferMaxBytes = nextPowerOf2(bufferSizeAdvice);
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
    sonStream = sonicCreateStream(m_sampleRate, m_channelCount);
    sonicSetSpeed(sonStream, m_speedFactor);
}


void PonyAudioSink::start() {
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
    qDebug() << "Audio start.";
    emit stateChanged();
}

void PonyAudioSink::pause() {
    if (m_state == PlaybackState::PLAYING) {
        Pa_StopStream(m_stream);
        m_state = PlaybackState::PAUSED;
    } else {
        throw std::runtime_error("AudioSink already paused.");
    }
    qDebug() << "Audio pause.";
}

void PonyAudioSink::stop() {
    if (m_state == PlaybackState::PLAYING || m_state == PlaybackState::PAUSED) {
        Pa_AbortStream(m_stream);
        m_state = PlaybackState::STOPPED;
    } else {
        qWarning() << "AudioSink already stopped.";
    }
}

PonyAudioSink::~PonyAudioSink() {
    m_state = PlaybackState::STOPPED;
    PaError err = Pa_CloseStream(m_stream);
    m_stream = nullptr;
    if (err != paNoError) {
        qWarning() << "Error at Destroying PonyAudioSink" << Pa_GetErrorText(err);
    }
}


PlaybackState PonyAudioSink::state() const {
    return m_state;
}

double PonyAudioSink::getProcessSecs() const {
    if (m_state == PlaybackState::STOPPED)
        return 0;
    return static_cast<double>((dataWritten - dataLastWrote) / (m_channelCount * m_bytesPerSample)) / (m_sampleRate) +
           timeBase;
}


void PonyAudioSink::setStartPoint(double t) {
    if (m_state == PlaybackState::STOPPED) {
        timeBase = t;
        dataWritten = 0;
        dataLastWrote = 0;
    } else {
        qWarning() << "setTimeBase make no effect when state != STOPPED";
    }

}

int PonyAudioSink::m_paCallback(const void *, void *outputBuffer, unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags) {
    ring_buffer_size_t bytesAvailCount = PaUtil_GetRingBufferReadAvailable(&ringBuffer);
    auto bytesNeeded = static_cast<ring_buffer_size_t>(framesPerBuffer * static_cast<unsigned long>(m_channelCount) *
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
                qint64 origLengthReduced = std::max(1ll, static_cast<qint64>(static_cast<double>(byteRemainToAlign) *
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
            transformVolume(outputBuffer, framesPerBuffer);
        } else {
            PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, byteToBeWritten);
            transformVolume(outputBuffer, framesPerBuffer);
        }
        dataWritten += timeAlignedByteWritten;
        dataLastWrote = timeAlignedByteWritten;
    }
    return paContinue;
}

size_t PonyAudioSink::freeByte() const {
    return static_cast<size_t>(PaUtil_GetRingBufferWriteAvailable(&ringBuffer));
}

bool PonyAudioSink::write(const char *buf, qint64 origLen) {

    int len = 0;
    int sonicBufferOffset = 0;
    switch (m_sampleFormat) {
        case PonySampleFormat::Int16:
            if (origLen % (m_channelCount * m_bytesPerSample) == 0) {
                sonicWriteShortToStream(sonStream, reinterpret_cast<const short *>(buf),
                                        static_cast<int>(origLen) / (m_channelCount * m_bytesPerSample));
                int currentLen = 0;
                while ((currentLen = sonicReadShortFromStream(sonStream,
                                                              reinterpret_cast<short *>
                                                              (sonicBuffer + len * m_channelCount),
                                                              0x7fffffff))) {
                    len += currentLen;
                }
            } else throw std::runtime_error("Incomplete Int16!");
            break;
            // TODO: implementation required.
        default:
            break;
    }
    len *= m_channelCount * m_bytesPerSample;
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

size_t PonyAudioSink::clear() {
    if (m_state != PlaybackState::STOPPED) {
        qWarning() << "clear make no effect when state != STOPPED.";
    }
    // 需要保证此刻没有读写操作
    PaUtil_FlushRingBuffer(&ringBuffer);
    return 0;
}

void PonyAudioSink::m_paStreamFinishedCallback() {
    if (m_state == PlaybackState::PLAYING) {
        emit resourceInsufficient();
    }
    m_state = PlaybackState::PAUSED;
    emit stateChanged();
    qDebug() << "Stream finished callback.";

}

void PonyAudioSink::transformVolume(void *buffer, unsigned long framesPerBuffer) const {
    unsigned long sampleCount = framesPerBuffer * static_cast<unsigned long>(m_channelCount);
    switch (m_sampleFormat) {
        case PonySampleFormat::Float:
            transformSampleVolume<float>(static_cast<std::byte *>(buffer), m_volume, sampleCount);
            break;
        case PonySampleFormat::Int16:
            transformSampleVolume<int16_t>(static_cast<std::byte *>(buffer), m_volume, sampleCount);
            break;
        case PonySampleFormat::Int32:
            transformSampleVolume<int32_t>(static_cast<std::byte *>(buffer), m_volume, sampleCount);
            break;
        case PonySampleFormat::UInt8:
            transformSampleVolume<uint8_t>(static_cast<std::byte *>(buffer), m_volume, sampleCount);
            break;
        default:
            throw std::runtime_error("Unsupported sample format!");
    }

}

void PonyAudioSink::setVolume(qreal newVolume) {
    m_volume = qBound(0.0, newVolume, 1.0);
}

qreal PonyAudioSink::volume() const {
    return m_volume;
}

template<typename T>
void PonyAudioSink::transformSampleVolume(std::byte *src_, qreal factor, unsigned long samples) const {
    T *src = static_cast<T *>(static_cast<void *>(src_));
    for (size_t sampleOffset = 0; sampleOffset < samples; sampleOffset++) {
        src[sampleOffset] = static_cast<T>(src[sampleOffset] * factor);
    }
}


PonyAudioFormat::PonyAudioFormat(PonySampleFormat sampleFormat_, int channelCount_, int sampleRate_) {
    setSampleFormat(sampleFormat_);
    setChannelCount(channelCount_);
    setSampleRate(sampleRate_);
}

size_t PonyAudioFormat::getBytesPerSample() const {
    return bytesPerSample;
}


void PonyAudioFormat::setSampleFormat(PonySampleFormat sampleFormat_) {
    sampleFormat = sampleFormat_;
    switch (sampleFormat) {
        case PonySampleFormat::Float:
            paSampleFormat = paFloat32;
            bytesPerSample = 4;
            break;
        case PonySampleFormat::Int16:
            paSampleFormat = paInt16;
            bytesPerSample = 2;
            break;
        case PonySampleFormat::Int32:
            paSampleFormat = paInt32;
            bytesPerSample = 4;
            break;
        case PonySampleFormat::UInt8:
            paSampleFormat = paUInt8;
            bytesPerSample = 1;
            break;
        case PonySampleFormat::Unknown:
        case PonySampleFormat::NSampleFormats:
            throw std::runtime_error("Unsupported audio format!");
    }
}

PaSampleFormat PonyAudioFormat::getSampleFormatForPA() const {
    return paSampleFormat;
}
