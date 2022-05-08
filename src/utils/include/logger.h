//
// Created by ColorsWind on 2022/3/24.
//

#ifndef SOFTWAREENGINEERING_LOGGER_H
#define SOFTWAREENGINEERING_LOGGER_H
#include <QtCore>
void initializeLogHandler();
void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#endif //SOFTWAREENGINEERING_LOGGER_H
