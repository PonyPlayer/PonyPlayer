//
// Created by ColorsWind on 2022/3/24.
//
#include <QDateTime>
#include <QMutex>
#include <QVector>
#include "logger.h"

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMap<QtMsgType, QString> types = {
            {QtDebugMsg, "DEBUG"},
            {QtWarningMsg, "WARN"},
            {QtInfoMsg, "INFO"},
            {QtFatalMsg, "FATAL"},
            {QtCriticalMsg, "CRITICAL"}
    };
    // TODO log filter
    // TODO log file, function, line
    // TODO writer to file
    QByteArray localMsg = msg.toLocal8Bit();
    QString currentDateTime = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
    QTextStream qTextStream(stderr);
    qTextStream << currentDateTime << " " << types[type] << ": " << localMsg << "\n";
    qTextStream.flush();
}