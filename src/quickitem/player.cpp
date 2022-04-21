//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"

void HurricanePlayer::openFile(const QString &path) {
    qDebug() << "open file" << path;
}

void HurricanePlayer::start() {
    qDebug() << "pause start";
}

void HurricanePlayer::pause() {
    qDebug() << "pause";
}

void HurricanePlayer::stop() {
    qDebug() << "stop";
}

void HurricanePlayer::seek(qreal sec) {
    qDebug() << "seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "setSpeed" << speed;
}
