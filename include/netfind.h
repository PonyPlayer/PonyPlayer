#pragma once

#include <utility>
#include <vector>
#include <string>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QUdpSocket>
#include "endpoint.h"
#include <QTimer>


class NetFindWorker : public QObject {
Q_OBJECT
private:
    QUdpSocket *socket;
    QByteArray datagram;
public:
    NetFindWorker();

public slots:

    void findNet();

signals:

    void foundNew(NetEndPoint endpoint);

};

class NetFind : public QObject {
Q_OBJECT

private:
    std::vector<NetEndPoint> endPoints;
    QThread *netFindThread;
    NetFindWorker *worker;
public:
    NetFind();


public slots:

    void start();

    void recvNetFind(NetEndPoint endpoint);

signals:

    void startFind();
};
