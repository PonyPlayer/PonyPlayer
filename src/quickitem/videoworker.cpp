//
// Created by ColorsWind on 2022/4/28.
//


#include "quickitem.h"


void VideoPlayWorker::syncTo(double pts) {
    auto duration = pts - audioOutput->getProcessSecs();
    if (duration > 0) {
        QThread::usleep(static_cast<unsigned long>(duration * 1000 * 1000));
    } else {
        qWarning() << "Sleep negative duration" << duration << "us";
    }
}

void VideoPlayWorker::slotOnWork() {
    qDebug() << "Start Video Work" << QThread::currentThread()->objectName();
    Picture currFrame;
    demuxer->start();
    pauseRequested = false;
    // ensure no noise
    for (int i = 0; i < 5 && audioOutput->freeByte() > MAX_AUDIO_FRAME_SIZE; ++i) {
        Sample sample = demuxer->getSample(true);
        demuxer->popSample(true);
        audioOutput->write(reinterpret_cast<const char *>(sample.data), sample.len);
        sample.free();
    }
    audioOutput->start();
    emit signalStateChanged(HurricaneState::PLAYING);
    while (!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
        demuxer->popPicture(true);
        emit signalImageChanged(currFrame);
        for (int i = 0; i < 10 && audioOutput->freeByte() > MAX_AUDIO_FRAME_SIZE; ++i) {
            Sample sample = demuxer->getSample(true);
            if (!sample.isValid()) {
                break;
            }
            demuxer->popSample(true);
            audioOutput->write(reinterpret_cast<const char *>(sample.data), sample.len);
            sample.free();
        }

        // sync volume
        qreal v = volume;
        audioOutput->setVolume(v);
        if (abs(audioOutput->volume() - v) >= 1e-5) {
            qWarning() << "Fail to set volume:" << v << "Current: " << audioOutput->volume();
            emit signalVolumeChangedFail(audioOutput->volume());
        }

        // sync frame
        Picture nextFrame = demuxer->getPicture(true);
        if (nextFrame.isValid() && !pauseRequested ) {
            syncTo(nextFrame.pts);
        }

    }
    qDebug() << "Stop work";
    audioOutput->pause();
    pauseRequested = true;
    demuxer->pause();
    emit signalStateChanged(HurricaneState::PAUSED);
}


void VideoPlayWorker::slotClose() {
    pauseRequested = true;
    demuxer->close();
    audioOutput->stop();
    audioOutput->clear();
    emit signalStateChanged(HurricaneState::INVALID);
}


void VideoPlayWorker::slotThreadInit() {
    // this function must be called on VideoPlayWorker's thread
    PonyAudioFormat format(PonySampleFormat::Int16, 2, 44100);
    audioOutput = new PonyAudioSink(format, MAX_AUDIO_FRAME_SIZE * 2);
//    connect(audioOutput, &QAudioSink::stateChanged, this, &VideoPlayWorker::onAudioStateChanged, Qt::ConnectionType::DirectConnection);
//    audioOutput->setVolume(100);
//    audioOutput->setBufferSize(MAX_AUDIO_FRAME_SIZE * 2);
}

//void VideoPlayWorker::onAudioStateChanged(QAudio::State state) {
//    auto curr = audioOutput->elapsedUSecs();
//    qDebug() << "QAudioSink State changed:" << audioOutput->state() << state << "Curr" << curr << "Sum" << idleDurationSum;
//    switch(state) {
//        case QAudio::ActiveState:
//            if (idlePoint > 0) {
//                idleDurationSum += curr - idlePoint;
//                idlePoint = -1;
//            } else {
//                qDebug() << "State changed to ActiveState, but fail to determine last time of IdleState or SuspendState.";
//                throw std::runtime_error("a");
//            }
//            break;
//        case QAudio::StoppedState:
//            if (audioOutput->error() != QAudio::NoError) {
//                qWarning() << "Error occurs" << audioOutput->error();
//            }
//            idlePoint = -1;
//            break;
//        case QAudio::SuspendedState:
//        case QAudio::IdleState:
//            // Start: IdleState -> ActiveState
//            // Pause: ActiveState -> SuspendedState
//            // Resume: SuspendedState -> IdleState -> ActiveState
//            if (idlePoint <= 0)
//                idlePoint = curr;
//            break;
//    }
//}



void VideoPlayWorker::slotSeek(qreal pos) {
    qDebug() << "Receive Seek request" << pos << QThread::currentThread()->objectName();
    audioOutput->stop();
    audioOutput->clear();


    // WARNING: must make sure everything (especially PTS) has been properly updated
    // otherwise, the video thread will be BLOCKING for a long time.
    emit signalDecoderSeek(static_cast<time_point>(pos * 1000 * 1000)); // blocking connection
    demuxer->flush();
    demuxer->start(); // non-blocking, make sure pic and sample request can be blocked

    // time-consuming job
    {
        Picture pic;
        while (pic = demuxer->getPicture(true), pic.getPTS() < pos) {
            demuxer->popPicture(true);
            pic.free();
        }
        emit signalImageChanged(demuxer->getPicture(true));
        demuxer->popPicture(true);
    }
    {
        Sample sample;
        while (sample = demuxer->getSample(true), sample.getPTS() < pos) {
            demuxer->popSample(true);
            sample.free();
        }
        qDebug() << sample.getPTS() << sample.isValid();
        audioOutput->setStartPoint(sample.getPTS());
    }

    emit signalPositionChangedBySeek();
    qDebug() << "Finished seek request" << pos;

}


void VideoPlayWorker::slotOpenFileResult(bool ret) {
    HurricaneState state;
    if (ret) {
        demuxer->start();
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
    audioOutput->setStartPoint();
    QUrl url(path);
    QString localPath = url.toLocalFile();
    qDebug() << "Open file" << localPath;
    emit signalDecoderOpenFile(localPath.toStdString());
}

VideoPlayWorker::VideoPlayWorker(QObject *parent) : QObject(nullptr) {
    videoThread = new QThread;
    videoThread->setObjectName("VideoThread");
    this->moveToThread(videoThread);
    videoThread->start();
    demuxer = new Demuxer(nullptr);
    // open file
    connect(this, &VideoPlayWorker::signalDecoderOpenFile, demuxer, &Demuxer::openFile);
    connect(demuxer, &Demuxer::openFileResult, this, &VideoPlayWorker::slotOpenFileResult);


    // seek
    connect(this, &VideoPlayWorker::signalDecoderSeek, demuxer, &Demuxer::seek, Qt::BlockingQueuedConnection);

}




