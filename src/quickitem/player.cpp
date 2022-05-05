//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer2.h"

#include <QUrl>
#include <QTimer>
#include <QDir>


//#define DEBUG_FLAG_AUTO_OPEN

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent) {
    videoPlayWorker = new VideoPlayWorker(this);
    connect(this, &HurricanePlayer::signalPlayerInitializing, videoPlayWorker, &VideoPlayWorker::slotThreadInit);
    connect(this, &HurricanePlayer::signalResume, videoPlayWorker, &VideoPlayWorker::slotOnWork);

    connect(this, &HurricanePlayer::signalOpenFile, videoPlayWorker, &VideoPlayWorker::slotOpenFile);
    connect(this, &HurricanePlayer::signalClose, videoPlayWorker, &VideoPlayWorker::slotClose);
    connect(videoPlayWorker, &VideoPlayWorker::signalImageChanged, this, &HurricanePlayer::setImage);
    connect(videoPlayWorker, &VideoPlayWorker::signalStateChanged, this, &HurricanePlayer::slotStateChanged);

    // volume
    connect(videoPlayWorker, &VideoPlayWorker::signalVolumeChangedFail, this, &HurricanePlayer::slotVolumeChangedFail);

    // seek
    connect(this, &HurricanePlayer::signalSeek, videoPlayWorker, &VideoPlayWorker::slotSeek);
    connect(videoPlayWorker, &VideoPlayWorker::signalPositionChangedBySeek, this, &HurricanePlayer::slotPositionChangedBySeek);

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
        state = HurricaneState::PLAYING;
        emit stateChanged();
        emit signalResume(QPrivateSignal());
        qDebug() << "Start play video.";
    }
}

void HurricanePlayer::pause() {
    if (state == HurricaneState::PLAYING || state == HurricaneState::PRE_PLAY) {
        state = HurricaneState::PRE_PAUSE;
        emit stateChanged();
        videoPlayWorker->pause();
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
    videoPlayWorker->pause();
    videoPlayWorker->deleteLater();
    qWarning() << "Destroy HurricanePlayer.";
}

void HurricanePlayer::slotStateChanged(HurricaneState s) {
    if (state == HurricaneState::LOADING) {
        emit openFileResult(state == HurricaneState::PAUSED);
    }
    state = s;
    emit stateChanged();
}

void HurricanePlayer::setVolume(qreal v) {
    qDebug() << "setVolume" << v;
    videoPlayWorker->setVolume(v);
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
    videoPlayWorker->pause();
    emit signalSeek(pos, QPrivateSignal());
    if (playing) {
        state = PRE_PLAY;
        emit stateChanged();
        emit signalResume(QPrivateSignal());
    }
    qDebug() << "HurricanePlayer: Seek" << pos;

}







