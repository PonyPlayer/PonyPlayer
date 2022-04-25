#include "netfind.h"
#include <QThread>

NetFind::NetFind() {
    netFindThread = new QThread(this);
    worker = new NetFindWorker;
    worker->moveToThread(netFindThread);
    connect(this, &NetFind::startFind, worker, &NetFindWorker::findNet);
    connect(worker, &NetFindWorker::foundNew, this, &NetFind::recvNetFind);
    netFindThread->start();
}

void NetFind::recvNetFind(NetEndPoint endpoint) {
    qDebug() << endpoint.getAddress();
}

void NetFind::start() {
    emit startFind();
}

NetFindWorker::NetFindWorker() {
    socket = new QUdpSocket();
    socket->bind(3366);
}

void NetFindWorker::findNet() {
    QHostAddress addr;
    qDebug() << "NetFindWorker started findnet";
    while (socket->hasPendingDatagrams()) {
        qDebug() << "NetFindWorker get pending datagram";
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size(), &addr);
        NetEndPoint endpoint(addr.toString(), addr.toString());
        emit foundNew(endpoint);
    }
}
