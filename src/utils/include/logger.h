//
// Created by ColorsWind on 2022/3/24.
//

#ifndef SOFTWAREENGINEERING_LOGGER_H
#define SOFTWAREENGINEERING_LOGGER_H
#include <QtCore>

namespace PonyPlayer {
    void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    QString getLogFile();
}
#endif //SOFTWAREENGINEERING_LOGGER_H
