//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer2.h"

#include <QUrl>
#include <QTimer>
#include <QDir>


//#define DEBUG_FLAG_AUTO_OPEN

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Fireworks(parent) {
//    videoPlayWorker = new VideoPlayWorker(this);
    frameController = new FrameController(this);
//    connect(this, &HurricanePlayer::signalPlayerInitializing, videoPlayWorker, &VideoPlayWorker::slotThreadInit);
//    connect(this, &HurricanePlayer::signalResume, videoPlayWorker, &VideoPlayWorker::slotOnWork);
    connect(this, &HurricanePlayer::signalStart, frameController, &FrameController::start);
    connect(this, &HurricanePlayer::signalPause, frameController, &FrameController::pause);
//    connect(this, &HurricanePlayer::sign)

//    connect(this, &HurricanePlayer::signalOpenFile, videoPlayWorker, &VideoPlayWorker::slotOpenFile);
//    connect(this, &HurricanePlayer::signalClose, videoPlayWorker, &VideoPlayWorker::slotClose);
    connect(this, &HurricanePlayer::signalOpenFile, frameController, &FrameController::openFile);
    connect(frameController, &FrameController::openFileResult, this, &HurricanePlayer::slotOpenFileResult);
    connect(this, &HurricanePlayer::signalClose, frameController, &FrameController::close);
    connect(frameController, &FrameController::setPicture, this, &HurricanePlayer::setImage);

    // volume
//    connect(videoPlayWorker, &VideoPlayWorker::signalVolumeChangedFail, this, &HurricanePlayer::slotVolumeChangedFail);

    // seek
//    connect(this, &HurricanePlayer::signalSeek, videoPlayWorker, &VideoPlayWorker::slotSeek);
//    connect(videoPlayWorker, &VideoPlayWorker::signalPositionChangedBySeek, this, &HurricanePlayer::slotPositionChangedBySeek);
    connect(this, &HurricanePlayer::signalSeek, frameController, &FrameController::seek);
    connect(frameController, &FrameController::signalPositionChangedBySeek, this, &HurricanePlayer::slotPositionChangedBySeek);

    connect(frameController, &FrameController::playbackStateChanged, this, &HurricanePlayer::slotPlaybackStateChanged);

    connect(this, &HurricanePlayer::stateChanged, [=]{
        qDebug() << "State Changed to" << QVariant::fromValue(state).toString();});
    emit signalPlayerInitializing(QPrivateSignal());
#ifdef DEBUG_FLAG_AUTO_OPEN
    openFile(QUrl::fromLocalFile(QDir::homePath().append(u"/581518754-1-208.mp4"_qs)).url());
#endif
}


void HurricanePlayer::openFile(const QString &path, bool autoClose) {
    if (autoClose && state == HurricaneState::PAUSED) {
        emit close();
    }
    if (state == HurricaneState::CLOSING || state == HurricaneState::INVALID) {
        if (state == HurricaneState::CLOSING) {
            // wait for the operation to complete
            QCoreApplication::processEvents();
        }
        state = HurricaneState::LOADING;
        emit stateChanged();
        emit signalOpenFile(path, QPrivateSignal());
        qDebug() << "Open file:" << path;
    }
}

void HurricanePlayer::start() {
    if (state == HurricaneState::PAUSED || state == HurricaneState::PRE_PAUSE) {
        state = HurricaneState::PRE_PLAY;
        emit stateChanged();
        emit signalStart(QPrivateSignal());
        qDebug() << "Start play video.";
    }
}

void HurricanePlayer::pause() {
    if (state == HurricaneState::PLAYING || state == HurricaneState::PRE_PLAY) {
        state = HurricaneState::PRE_PAUSE;
        emit stateChanged();
        emit signalPause(QPrivateSignal());
        qDebug() << "Pause.";
    }
}


void HurricanePlayer::close() {
    if (state == HurricaneState::PRE_PAUSE || state == HurricaneState::PAUSED) {
        state = HurricaneState::CLOSING;
        emit stateChanged();
        this->setImage(Picture());
        emit signalClose(QPrivateSignal());
    }
}

HurricanePlayer::~HurricanePlayer() {
//    videoPlayWorker->pause();
//    videoPlayWorker->deleteLater();
    frameController->pause();
    frameController->deleteLater();
    qWarning() << "Destroy HurricanePlayer.";
}

void HurricanePlayer::setVolume(qreal v) {
    qDebug() << "setVolume" << v;
//    videoPlayWorker->setVolume(v);
}

void HurricanePlayer::seek(qreal pos) {
    // only available on PLAY/PAUSE
    bool playing = false;
    switch(state) {
        case HurricaneState::PLAYING:
        case HurricaneState::PRE_PLAY:
            playing = true;
            /* fall through */
        case HurricaneState::PAUSED:
        case HurricaneState::PRE_PAUSE:
            break;
        default:
            return;
    }
    if (pos < 0 || pos > getVideoDuration())
        return;
    state = PRE_PAUSE;
    emit stateChanged();
//    emit signalPause(QPrivateSignal());
    emit signalSeek(pos, QPrivateSignal());
//    if (playing) {
//        emit signalStart(QPrivateSignal());
//    }
    qDebug() << "HurricanePlayer: Seek" << pos;

}







