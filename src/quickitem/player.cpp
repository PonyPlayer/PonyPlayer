//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent), videoPlayThread(this, &this->demuxer, parent) {
    videoPlayThread.start();
    connect(this, &HurricanePlayer::videoStart, &videoPlayThread, &VideoPlayThread::videoStart);
    connect(this, &HurricanePlayer::videoPause, &videoPlayThread, &VideoPlayThread::videoPause);
    connect(&videoPlayThread, &VideoPlayThread::setImage, this, &HurricanePlayer::setImage);
}


void HurricanePlayer::openFile(const QString &path) {
    QUrl url(path);
    demuxer.openFile(url.path().toStdString());
    demuxer.initDemuxer();
    qDebug() << "HP: open file" << url.path();
    emit start();
}

void HurricanePlayer::start() {
    qDebug() << "HP: start";
    emit videoStart();
}

void HurricanePlayer::pause() {
    qDebug() << "HP: pause";
    emit videoPause();
}

void HurricanePlayer::stop() {
    qDebug() << "HP: stop";
    emit videoPause();
}


void HurricanePlayer::seek(qreal sec) {
    qDebug() << "HP: seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "HP: setSpeed" << speed;
}

