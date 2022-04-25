#include "broadcast.h"
#include <QThread>


BroadCaster::BroadCaster() {
    QList<QNetworkAddressEntry> entrys = NetEndPoint::getBroadCastEntrys();
    for (const auto &entry: entrys) {
        if (!entry.broadcast().isNull()) {
            auto *workerThread = new QThread;
            auto *worker = new BroadCastWorker(entry.ip(), entry.broadcast());
            worker->moveToThread(workerThread);
            workerThread->start();
            workerThreads.push_back(workerThread);
            connect(this, &BroadCaster::broadcast, worker, &BroadCastWorker::broadcast);
            qDebug() << "Binding: " << entry.broadcast();
        }
    }
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BroadCaster::timedBroadcast);
}

void BroadCaster::timedBroadcast() {
    emit broadcast();
}

void BroadCaster::startBroadcast() {
    timer->start(5000);
}

void BroadCaster::stopBroadcast() {
    timer->stop();
}

void BroadCastWorker::broadcast() {
    QString broadcastContent = (QString("PONYPLAYERIP ") + ip.toString());
    QByteArray datagram = broadcastContent.toUtf8();
    socket->writeDatagram(datagram, broadcastIp, 3366);
    qDebug() << "Broadcasting: " << broadcastContent;
}

BroadCastWorker::BroadCastWorker(const QHostAddress &ip, const QHostAddress &broadcastIp) : ip(ip),
                                                                                            broadcastIp(broadcastIp) {
    socket = new QUdpSocket;
    socket->bind(broadcastIp);

}
