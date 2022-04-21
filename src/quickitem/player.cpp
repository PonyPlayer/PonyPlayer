//
// Created by ColorsWind on 2022/4/21.
//
#include "quickitem.h"

void HurricanePlayer::openFile(const QString &path) {
    qDebug() << "HP: open file" << path;
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

HurricanePlayer::HurricanePlayer(QQuickItem *parent) : Hurricane(parent) {}

void HurricanePlayer::seek(qreal sec) {
    qDebug() << "HP: seek" << sec;
}

void HurricanePlayer::setSpeed(qreal speed) {
    qDebug() << "HP: setSpeed" << speed;
}
