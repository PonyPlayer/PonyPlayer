#pragma once

#include <QHostInfo>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QTimer>
#include "endpoint.h"


class BroadCastWorker : public QObject {
Q_OBJECT
private:
    QHostAddress ip, broadcastIp;
    QUdpSocket *socket;
public:
    BroadCastWorker(const QHostAddress &ip, const QHostAddress &broadcastIp);

public slots:

    void broadcast();

};

class BroadCaster : public QObject {
Q_OBJECT
private:
    QList<QThread *> workerThreads;
    QTimer *timer;

public:
    BroadCaster();

public slots:

    void timedBroadcast();

    void startBroadcast();

    void stopBroadcast();

signals:

    void broadcast();
};