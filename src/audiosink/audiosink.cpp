#include "audiosink.h"
#include "helper.h"


PonyAudioSink::PonyAudioSink(QAudioFormat format, unsigned long bufferSizeAdvice)
        : m_stream(nullptr), timeBase(0), prevPlayTime(0),
          m_state(PlaybackState::IDLED) {
    Pa_Initialize();
    param = new PaStreamParameters;
    param->device = Pa_GetDefaultOutputDevice();
    if (param->device == paNoDevice)
        throw std::runtime_error("no audio device!");
    m_channelCount = format.channelCount();
    param->channelCount = m_channelCount;
    param->sampleFormat = qSampleFormatToPortFormat(format.sampleFormat(), m_bytesPerSample);
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
            &PonyAudioSink::paCallback,
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
    Pa_SetStreamFinishedCallback(&m_stream, &PonyAudioSink::paStreamFinished);
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
    PaError err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        qWarning() << "Error" << Pa_GetErrorText(err);
        throw std::runtime_error("can not start stream!");
    }
    prevPlayTime = 0;
    timeBase = Pa_GetStreamTime(m_stream);
    m_state = PlaybackState::PLAYING;
    emit stateChanged();
}

PonyAudioSink::~PonyAudioSink() {
    m_state = PlaybackState::STOPPED;
    PaError err = Pa_CloseStream(m_stream);
    m_stream = nullptr;
    if (err != paNoError)
        throw std::runtime_error("close audio failed!");
}

int PonyAudioSink::paCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
    return static_cast<PonyAudioSink *>(userData)->m_paCallback(inputBuffer, outputBuffer,
                                                                framesPerBuffer,
                                                                timeInfo, statusFlags);
}

void PonyAudioSink::paStreamFinished(void *userData) {
    static_cast<PonyAudioSink *>(userData)->m_paStreamFinishedCallback();
    qDebug() << "audio stream finished";
}

void PonyAudioSink::stop() {
    Pa_StopStream(m_stream);
}

void PonyAudioSink::abort() {
    Pa_AbortStream(m_stream);
}

PlaybackState PonyAudioSink::state() const {
    return m_state;
}

double PonyAudioSink::getProcessSecs() const {
    switch (m_state) {
        case PlaybackState::PLAYING:
            return Pa_GetStreamTime(m_stream) - timeBase + prevPlayTime;
        case PlaybackState::IDLED:
            return prevPlayTime;
        case PlaybackState::STOPPED:
            return 0;
    }
    return 0;
}


void PonyAudioSink::setProcessSecs(double t) {
    switch (m_state) {
        case PlaybackState::PLAYING:
            timeBase = Pa_GetStreamTime(m_stream) + prevPlayTime - t;
        case PlaybackState::IDLED:
            prevPlayTime = t;
        case PlaybackState::STOPPED:
            throw std::runtime_error("trying to set process secs on a stopped audio stream!");
    }
}

int PonyAudioSink::m_paCallback(const void *, void *outputBuffer, unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags) {
    ring_buffer_size_t bytesAvailCount = PaUtil_GetRingBufferReadAvailable(&ringBuffer);
    unsigned long bytesNeeded = framesPerBuffer * static_cast<unsigned long>(m_channelCount) * m_bytesPerSample;
    ring_buffer_size_t bytesToPlay = std::min(
            static_cast<ring_buffer_size_t> (bytesNeeded),
            static_cast<ring_buffer_size_t> (bytesAvailCount));
    if (bytesToPlay != bytesNeeded) {
        if (m_state == PlaybackState::PLAYING) {
            m_state = PlaybackState::IDLED;
            prevPlayTime += timeInfo->currentTime - timeBase;
            memset(outputBuffer, 0, bytesNeeded);
            emit stateChanged();
        }
        timeBase = timeInfo->currentTime;
    } else {
        if (m_state == PlaybackState::IDLED) {
            m_state = PlaybackState::PLAYING;
            emit stateChanged();
        }
        PaUtil_ReadRingBuffer(&ringBuffer, outputBuffer, bytesToPlay);
    }
    return 0;
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
    // 需要保证此刻没有读写操作
    PaUtil_FlushRingBuffer(&ringBuffer);
    return 0;
}

void PonyAudioSink::m_paStreamFinishedCallback() {
    m_state = PlaybackState::STOPPED;
    prevPlayTime = 0;
    emit stateChanged();
}
