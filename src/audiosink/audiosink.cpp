#include "audiosink.h"
#include "helper.h"

PonyAudioSink::PonyAudioSink(QAudioFormat format, unsigned long bufferSizeAdvice)
        : m_stream(nullptr), timeBase(0), volume(1.0),
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
    if (param->device == paNoDevice)
        throw std::runtime_error("no audio device!");
    m_channelCount = format.channelCount();
    m_sampleFormat = qSampleFormatToPortFormat(format.sampleFormat(), m_bytesPerSample);
    param->channelCount = m_channelCount;
    param->sampleFormat = m_sampleFormat;
    param->suggestedLatency = Pa_GetDeviceInfo(param->device)->defaultLowOutputLatency;
    param->hostApiSpecificStreamInfo = nullptr;
    m_sampleRate = format.sampleRate();
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
    ringBufferData = PaUtil_AllocateMemory(static_cast<long>(m_bufferMaxBytes));
    if (PaUtil_InitializeRingBuffer(&ringBuffer,
                                    sizeof(std::byte),
                                    static_cast<ring_buffer_size_t>(m_bufferMaxBytes),
                                    ringBufferData) < 0)
        throw std::runtime_error("can not initialize ring buffer!");
    Pa_SetStreamFinishedCallback(&m_stream, [](void *userData) {
        static_cast<PonyAudioSink *>(userData)->m_paStreamFinishedCallback();
    });
}

PaSampleFormat PonyAudioSink::qSampleFormatToPortFormat(QAudioFormat::SampleFormat qFormat, size_t &numBytes) {
    switch (qFormat) {
        case QAudioFormat::SampleFormat::Float:
            numBytes = 4;
            return paFloat32;
        case QAudioFormat::SampleFormat::Int16:
            numBytes = 2;
            return paInt16;
        case QAudioFormat::SampleFormat::Int32:
            numBytes = 4;
            return paInt32;
        case QAudioFormat::SampleFormat::UInt8:
            numBytes = 1;
            return paUInt8;
        case QAudioFormat::SampleFormat::Unknown:
        case QAudioFormat::SampleFormat::NSampleFormats:
            throw std::runtime_error("unknown audio format!");
    }
    return paCustomFormat;
}

void PonyAudioSink::start() {
    if (m_state == PlaybackState::PLAYING) {
        throw std::runtime_error("Already start.");
    }
    PaError err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        qWarning() << "Error at starting stream" << Pa_GetErrorText(err);
        throw std::runtime_error("Can not start stream!");
    }
    if (m_state == PlaybackState::STOPPED) {
        timeBase = Pa_GetStreamTime(m_stream) - timeBase;
    }
    m_state = PlaybackState::PLAYING;
    emit stateChanged();
}

void PonyAudioSink::pause() {
    if (m_state == PlaybackState::PLAYING) {
        Pa_StopStream(m_stream);
        m_state = PlaybackState::PAUSED;
    } else {
        throw std::runtime_error("PonyAudioSink: Already paused.");
    }
}

void PonyAudioSink::stop() {
    if (m_state == PlaybackState::PLAYING || m_state == PlaybackState::PAUSED) {
        Pa_AbortStream(m_stream);
        m_state = PlaybackState::STOPPED;
    } else {
        throw std::runtime_error("PonyAudioSink: Already stopped.");
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
    return Pa_GetStreamTime(m_stream) - timeBase;
}


void PonyAudioSink::setStartPoint(double t) {
    if (m_state == PlaybackState::STOPPED) {
        timeBase = t;
    } else {
        qWarning() << "setTimeBase make no effect when state != STOPPED";
    }

}

int PonyAudioSink::m_paCallback(const void *, void *outputBuffer, unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags) {
    ring_buffer_size_t bytesAvailCount = PaUtil_GetRingBufferReadAvailable(&ringBuffer);
    auto bytesNeeded = static_cast<ring_buffer_size_t>(framesPerBuffer * m_channelCount * m_bytesPerSample);
    if (bytesNeeded > bytesAvailCount) {
        PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, bytesAvailCount);
        memset(static_cast<std::byte *>(outputBuffer) + bytesAvailCount, 0, bytesNeeded - bytesAvailCount);
        transformVolume(outputBuffer, framesPerBuffer);
        return paComplete;
    } else {
        PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, bytesNeeded);
        transformVolume(outputBuffer, framesPerBuffer);
        return paContinue;
    }
}

size_t PonyAudioSink::freeByte() const {
    return static_cast<size_t>(PaUtil_GetRingBufferWriteAvailable(&ringBuffer));
}

bool PonyAudioSink::write(const char *buf, qint64 len) {
    ring_buffer_size_t bufAvailCount = PaUtil_GetRingBufferWriteAvailable(&ringBuffer);
    if (bufAvailCount < len) return false;
    void *ptr[2] = {nullptr};
    ring_buffer_size_t sizes[2] = {0};

    PaUtil_GetRingBufferWriteRegions(&ringBuffer, static_cast<ring_buffer_size_t>(len), &ptr[0], &sizes[0], &ptr[1],
                                     &sizes[1]);
    memcpy(ptr[0], buf, static_cast<size_t>(sizes[0]));
    memcpy(ptr[1], buf + sizes[0], static_cast<size_t>(sizes[1]));
    PaUtil_AdvanceRingBufferWriteIndex(&ringBuffer, static_cast<ring_buffer_size_t>(len));
    return true;
}

size_t PonyAudioSink::clear() {
    if (m_state != PlaybackState::STOPPED) {
        throw std::runtime_error("clear");
    }
    // 需要保证此刻没有读写操作
    PaUtil_FlushRingBuffer(&ringBuffer);
    return 0;
}

void PonyAudioSink::m_paStreamFinishedCallback() {
    if (m_state == PlaybackState::PLAYING) {
        emit forceStopped();
    }
    m_state = PlaybackState::STOPPED;
    emit stateChanged();
}

void PonyAudioSink::transformVolume(void *buffer, unsigned long framesPerBuffer) const {
    for (size_t frameOffset = 0; frameOffset < framesPerBuffer; frameOffset++) {
        for (int channelOffset = 0; channelOffset < m_channelCount; channelOffset++) {
            switch (m_sampleFormat) {
                case paFloat32:
                {
                    auto *sample = reinterpret_cast<float *>(static_cast<std::byte *> (buffer) +
                                                              (frameOffset * m_channelCount + channelOffset) *
                                                              m_bytesPerSample);
                    *sample *= static_cast<float>(volume);
                    break;
                }
                case paInt16:
                {
                    auto *sample = reinterpret_cast<int16_t *>(static_cast<std::byte *> (buffer) +
                                                              (frameOffset * m_channelCount + channelOffset) *
                                                              m_bytesPerSample);
                    *sample *= volume;
                    break;
                }
                case paInt32:
                {
                    auto *sample = reinterpret_cast<int32_t *>(static_cast<std::byte *> (buffer) +
                                                               (frameOffset * m_channelCount + channelOffset) *
                                                               m_bytesPerSample);
                    *sample *= volume;
                    break;
                }
                case paUInt8:
                {
                    auto *sample = reinterpret_cast<uint8_t *>(static_cast<std::byte *> (buffer) +
                                                               (frameOffset * m_channelCount + channelOffset) *
                                                               m_bytesPerSample);
                    *sample *= volume;
                    break;
                }
            }
        }
    }

}

void PonyAudioSink::setVolume(qreal newVolume) {
    volume = newVolume;
}
