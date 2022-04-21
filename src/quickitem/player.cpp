//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>

void HurricanePlayer::openFile(const QString &path) {
    QUrl url(path);
    auto *timer = new QTimer(this);
    timer->setInterval(1000/30);
    auto *demuxer = new Demuxer;
    demuxer->openFile(QDir::homePath().append(u"/143468776-1-208.mp4"_qs).toStdString());
    demuxer->initDemuxer();
    connect(timer, &QTimer::timeout, [=]{
        Picture pic = demuxer->getPicture();
        this->setImage(pic);
//        demuxer->videoFrameQueuePop();
    });
    timer->start();
    qDebug() << "HP: open file" << url.path();
}

void HurricanePlayer::start() {
    qDebug() << "HP: pause start";
}

void HurricanePlayer::pause() {
    qDebug() << "HP: pause";
}

void HurricanePlayer::stop() {
    qDebug() << "HP: stop";
}

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent) {
    auto *timer = new QTimer(this);
    timer->setInterval(1000/30);
    auto *demuxer = new Demuxer;
    demuxer->openFile(QDir::homePath().append(u"/143468776-1-208.mp4"_qs).toStdString());
    demuxer->initDemuxer();
    connect(timer, &QTimer::timeout, [=]{
        Picture pic = demuxer->getPicture();
        this->setImage(pic);
//        demuxer->videoFrameQueuePop();
    });
    timer->start();
}

void HurricanePlayer::seek(qreal sec) {
    qDebug() << "HP: seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "HP: setSpeed" << speed;
}
