#include "endpoint.h"

#include <utility>

NetEndPoint::NetEndPoint(QString _address, QString _name) : address(std::move(_address)),
                                                                    name(std::move(_name)) {

}

QList<QNetworkAddressEntry> NetEndPoint::getBroadCastEntrys() {
    QList<QNetworkAddressEntry> retEntryList;
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    for (int i = 0; i < interfaceList.count(); i++) {
        const QNetworkInterface& interf = interfaceList.at(i);
        QList<QNetworkAddressEntry> entryList = interf.addressEntries();
        for (int j = 0; j < entryList.count(); j++) {
            const QNetworkAddressEntry &entry = entryList.at(j);
            if (!entry.ip().isGlobal() || entry.ip().isNull()) continue;
            retEntryList.push_back(entry);
        }
    }
    return retEntryList;
}


