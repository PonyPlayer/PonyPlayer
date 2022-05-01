//
// Created by ColorsWind on 2022/4/28.
//


#include "quickitem.h"

time_point VideoPlayWorker::getAudioPlayingUSecs() {
    return audioOutput->elapsedUSecs() - idleDurationSum;
}

void VideoPlayWorker::syncTo(double pts) {
    auto target = static_cast<int64_t>(pts * 1000 * 1000); // us
    auto current = getAudioPlayingUSecs() + seekPoint; // us
    auto duration = target - current;
    if (duration > 0) {
        QThread::usleep(static_cast<unsigned long>(duration));
    } else {
        qWarning() << "Sleep negative duration" << duration << "us";
    }
}

void VideoPlayWorker::slotOnWork() {
    qDebug() << "Start Video Work" << QThread::currentThread()->objectName();
    Picture currFrame;
    pauseRequested = false;
    audioOutput->resume();
    emit signalStateChanged(HurricaneState::PLAYING);
    while(!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
        demuxer->popPicture(true);
        emit signalImageChanged(currFrame);
        for (int i = 0; i < 5 && audioOutput->bytesFree() > MAX_AUDIO_FRAME_SIZE; ++i) {
            try {
                Sample sample = demuxer->getSample(true);
                demuxer->popSample(true);
                audioInput->write(reinterpret_cast<const char *>(sample.data), sample.len);
                sample.free();
            } catch (std::runtime_error &ex) {
                qWarning() << "Fail to get sample" << ex.what();
                break;
            }
        }
        // process all events such as setVolume and pause request
        QCoreApplication::processEvents();
        Picture nextFrame = demuxer->getPicture(true);
        if (nextFrame.isValid()) {
            syncTo(nextFrame.pts);
        }

    }
    qDebug() << "Stop work";
    audioOutput->suspend();
    pauseRequested = true;
    emit signalStateChanged(HurricaneState::PAUSED);
}


void VideoPlayWorker::slotClose() {
    pauseRequested = true;
    demuxer->close();
    if (audioInput) {
        audioOutput->stop();
        audioInput = nullptr;
    }
    emit signalStateChanged(HurricaneState::INVALID);
}


void VideoPlayWorker::slotPause() {
    pauseRequested = true;
    emit signalStateChanged(HurricaneState::PAUSED);
}

void VideoPlayWorker::closeAudio() {
    // close audio and reset idleDurationSum
    audioOutput->stop();
    audioInput = nullptr;
    idleDurationSum = 0;
}
void VideoPlayWorker::slotThreadInit() {
    // this function must be called on VideoPlayWorker's thread
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);
    audioOutput = new QAudioSink(format, this);
    connect(audioOutput, &QAudioSink::stateChanged, this, &VideoPlayWorker::onAudioStateChanged, Qt::ConnectionType::DirectConnection);
    audioOutput->setVolume(100);
    audioOutput->setBufferSize(MAX_AUDIO_FRAME_SIZE * 2);
}

void VideoPlayWorker::onAudioStateChanged(QAudio::State state) {
    auto curr = audioOutput->elapsedUSecs();
    qDebug() << "QAudioSink State changed:" << audioOutput->state() << state << "Curr" << curr << "Sum" << idleDurationSum;
    switch(state) {
        case QAudio::ActiveState:
            if (idlePoint > 0) {
                idleDurationSum += curr - idlePoint;
                idlePoint = -1;
            } else {
                qDebug() << "State changed to ActiveState, but fail to determine last time of IdleState or SuspendState.";
                throw std::runtime_error("a");
            }
            break;
        case QAudio::StoppedState:
            if (audioOutput->error() != QAudio::NoError) {
                qWarning() << "Error occurs" << audioOutput->error();
            }
            idlePoint = -1;
            break;
        case QAudio::SuspendedState:
        case QAudio::IdleState:
            // Start: IdleState -> ActiveState
            // Pause: ActiveState -> SuspendedState
            // Resume: SuspendedState -> IdleState -> ActiveState
            if (idlePoint <= 0)
                idlePoint = curr;
            break;
    }
}



void VideoPlayWorker::slotSeek(qreal pos) {
    qDebug() << "Receive Seek request" << pos << QThread::currentThread()->objectName();
    // 1. Stop audio and clean buffer (invoke closeAudio)
    // 2. Set seekPoint so syncTo can handle pts correctly
    // 3. Clear invalid frame
    auto t = static_cast<time_point>(pos * 1000 * 1000);
    bool suspended = audioOutput->state() == QAudio::SuspendedState;
    seekPoint = t;
    qDebug() << "Set seek point" << t / 1000 / 1000 << QThread::currentThread()->objectName();
    closeAudio();

    // WARNING: must make sure everything (especially PTS) has been properly updated
    // otherwise, the video thread will be BLOCKING for a long time.
    demuxer->interrupt();
    emit signalDecoderSeek(t); // blocking connection
    demuxer->flush();
    emit signalDecoderStart(); // queue connection

    // time-consuming job
    {
        Picture pic;
        while(pic = demuxer->getPicture(true), pic.getPTS() < pos) {
            demuxer->popPicture(true);
            pic.free();
        }
    }
    {
        Sample sample;
        while(sample = demuxer->getSample(true), sample.getPTS() < pos) {
            demuxer->popSample(true);
            sample.free();
        }
    }
    audioInput = audioOutput->start();
    if (suspended)
        audioOutput->suspend();
    emit signalPositionChangedBySeek();
    qDebug() << "Finished seek request" << pos;

}

void VideoPlayWorker::slotVolumeChanged(qreal v) {
    audioOutput->setVolume(v);
    qreal current = audioOutput->volume();
    if (abs(current - v) >= 1e-5) {
        qWarning() << "Fail to set volume:" << v << "Current: " << current;
        emit signalVolumeChangedFail(current);
    }
}

void VideoPlayWorker::slotOpenFileResult(bool ret) {
    HurricaneState state;
    if (ret) {
        emit signalDecoderStart();
        state = HurricaneState::PAUSED;
        Picture pic = demuxer->getPicture(true);
        emit signalImageChanged(pic);
    } else {
        state = HurricaneState::INVALID;
        qWarning() << "Fail to open video." << ret;
    }
    emit signalStateChanged(state);
}

void VideoPlayWorker::slotOpenFile(const QString &path) {
    seekPoint = 0;
    idleDurationSum = 0;
    audioInput = audioOutput->start();
    audioOutput->suspend();
    QUrl url(path);
    QString localPath = url.toLocalFile();
    qDebug() << "Open file" << localPath;
    emit signalDecoderOpenFile(localPath.toStdString());
}

VideoPlayWorker::VideoPlayWorker() {
    demuxer = new Demuxer2;
    // open file
    connect(this, &VideoPlayWorker::signalDecoderOpenFile, demuxer, &Demuxer2::openFile);
    connect(demuxer, &Demuxer2::openFileResult, this, &VideoPlayWorker::slotOpenFileResult);

    // seek
    connect(this, &VideoPlayWorker::signalDecoderSeek, demuxer, &Demuxer2::seek, Qt::BlockingQueuedConnection);

    // start
    connect(this, &VideoPlayWorker::signalDecoderStart, demuxer, &Demuxer2::start);
}



