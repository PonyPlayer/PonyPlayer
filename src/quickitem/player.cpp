//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>


#define DEBUG_FLAG_AUTO_OPEN
HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent), demuxer(), videoPlayWorker(&demuxer) {
    videoThread = new QThread;
    videoThread->setObjectName("VideoThread");
    videoPlayWorker.moveToThread(videoThread);
    videoThread->start();
    connect(this, &HurricanePlayer::videoInit, &videoPlayWorker, &VideoPlayWorker::initOnThread);
    connect(this, &HurricanePlayer::videoStart, &videoPlayWorker, &VideoPlayWorker::onWork);
    connect(this, &HurricanePlayer::videoPause, &videoPlayWorker, &VideoPlayWorker::pause);
    connect(&videoPlayWorker, &VideoPlayWorker::setImage, this, &HurricanePlayer::setImage);
    connect(videoThread, &QThread::destroyed, []{ qDebug() << "Video Thread delete.";});
    emit videoInit();
#ifdef DEBUG_FLAG_AUTO_OPEN
    openFile(QUrl::fromLocalFile(QDir::homePath().append(u"/143468776-1-208.mp4"_qs)).url());
#endif
}


void HurricanePlayer::openFile(const QString &path) {
    QUrl url(path);
    qDebug() << "HP: open file" << path << QUrl(path).toLocalFile();
    demuxer.openFile(url.toLocalFile().toStdString());
    demuxer.initDemuxer();

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
    demuxer.quit();
    videoPlayWorker.pause();
    videoThread->quit();
    videoThread->deleteLater();
}


