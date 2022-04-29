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
        demuxer->popPicture();
        emit signalImageChanged(currFrame);
        for (int i = 0; i < 5 && audioOutput->bytesFree() > 19200; ++i) {
            Sample sample = demuxer->getSample(true);
            audioInput->write(reinterpret_cast<const char *>(sample.data), sample.len);
            demuxer->popSample();
            sample.free();
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


void VideoPlayWorker::slotOpenFile(const QString &path) {
    seekPoint = 0;
    idleDurationSum = 0;
    audioInput = audioOutput->start();
    audioOutput->suspend();
    QUrl url(path);
    QString localPath = url.toLocalFile();
    qDebug() << "Open file" << localPath;
    int ret = demuxer->openFile(localPath.toStdString());
    HurricaneState state;
    if (ret) {
        state = HurricaneState::PAUSED;
        Picture pic = demuxer->getPicture(true);
        emit signalImageChanged(pic);
    } else {
        state = HurricaneState::INVALID;
        qWarning() << "Fail to open video." << ret;
    }
    emit signalStateChanged(state);
}

void VideoPlayWorker::slotClose() {
    pauseRequested = true;
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
    audioOutput->setBufferSize(19200 * 10);
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
    bool isSuspended = audioOutput->state() == QAudio::SuspendedState;
    seekPoint = t;
    closeAudio();

    // time-consuming task
    demuxer->seek(t);
    while(demuxer->getPicture(true).getPTS() < pos) {
        demuxer->popPicture();
    }
    qDebug() << "Current Frame" << demuxer->getPicture(true).getPTS();
    while(demuxer->getSample(true).getPTS() < pos) {
        demuxer->popSample();
    }
    audioInput = audioOutput->start();
    if (isSuspended)
        audioOutput->suspend();
    emit signalPositionChangedBySeek();
    qDebug() << "Finished seek request" << pos;
}
