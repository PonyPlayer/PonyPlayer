//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"
#include "demuxer.h"

#include <QUrl>
#include <QTimer>
#include <QDir>
#include <QThread>
#include <QCu

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent) {
    timer.start();
}


void HurricanePlayer::openFile(const QString &path) {
    QUrl url(path);
    demuxer.openFile(url.path().toStdString());

    demuxer.initDemuxer();
    connect(&timer, &VideoTimer::timeEmit, this, &HurricanePlayer::playNextFrame, Qt::BlockingQueuedConnection);
    videoStartTime = clock();
    qDebug() << "HP: open file" << url.path();
    emit start();
}

void HurricanePlayer::playNextFrame() {
    qDebug() << "next frame";
    this->setImage(picture);
    Picture pic = demuxer.getPicture();
    if (pic.isValid()) {
        timer.frameEvent.block = false;
        timer.frameEvent.next = videoStartTime + static_cast<clock_t>(pic.pts * 1000);
        picture = pic;
    } else {
        timer.frameEvent.block = true;
    }
}

void HurricanePlayer::start() {
    Picture pic = demuxer.getPicture();
    if (pic.isValid()) {
        timer.frameEvent.block = false;
        timer.frameEvent.next = videoStartTime + static_cast<clock_t>(pic.pts * 1000);
        timer.notify();
    } else {

    }

    qDebug() << "HP: start";
}

void HurricanePlayer::pause() {
    qDebug() << "HP: pause";
}

void HurricanePlayer::stop() {
    qDebug() << "HP: stop";
}


void HurricanePlayer::seek(qreal sec) {
    qDebug() << "HP: seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "HP: setSpeed" << speed;
}

VideoTimer::VideoTimer(QObject *parent) : QThread(parent) {

}

void VideoTimer::run() {
    while (true) {
        qDebug() << "R";
        if (frameEvent.block) {
            mutex.lock();
            condition.wait(&mutex);
        } else {
            clock_t curr = clock();
            if (frameEvent.next > curr) {
                uint64_t delay = frameEvent.next - curr;
                qDebug() << delay;
                QThread::msleep(static_cast<unsigned long>(delay));
            }
            emit timeEmit();
        }
    }
}
