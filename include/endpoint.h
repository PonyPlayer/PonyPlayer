#pragma once

#include <utility>
#include <vector>
#include <string>
#include <QHostInfo>
#include <QNetworkInterface>

class NetEndPoint {
private:
    QString address;
    QString name;
public:
    NetEndPoint(QString _address, QString _name);

    static QList<QNetworkAddressEntry> getBroadCastEntrys();

    QString getAddress() { return address; }

    QString getName() { return name; }
};