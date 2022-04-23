//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent), demuxer(), videoPlayWorker(&demuxer, this) {
    videoThread = new QThread;
    videoThread->setObjectName("VideoThread");
    videoThread->start();

    videoPlayWorker.moveToThread(videoThread);
    connect(this, &HurricanePlayer::videoStart, &videoPlayWorker, &VideoPlayWorker::onWork);
    connect(this, &HurricanePlayer::videoPause, &videoPlayWorker, &VideoPlayWorker::pause);
    connect(&videoPlayWorker, &VideoPlayWorker::setImage, this, &HurricanePlayer::setImage);
    connect(videoThread, &QThread::destroyed, []{ qDebug() << "Video Thread delete.";});
}


void HurricanePlayer::openFile(const QString &path) {
    QUrl url(path);
    demuxer.openFile(url.toLocalFile().toStdString());
    demuxer.initDemuxer();
    qDebug() << "HP: open file" << url.toLocalFile();
    emit start();
}

void HurricanePlayer::start() {
    qDebug() << "HP: start";
    videoPlayWorker.resume();
    emit videoStart();
}

void HurricanePlayer::pause() {
    qDebug() << "HP: pause";
    videoPlayWorker.pause();
}

void HurricanePlayer::stop() {
    qDebug() << "HP: stop";
    videoPlayWorker.pause();
}


void HurricanePlayer::seek(qreal sec) {
    qDebug() << "HP: seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "HP: setSpeed" << speed;
}

HurricanePlayer::~HurricanePlayer() {
    videoPlayWorker.pause();
    videoThread->quit();
    videoThread->wait(100);
    qDebug() << "wait to quit" << videoThread->isRunning();
    delete videoThread;
}


