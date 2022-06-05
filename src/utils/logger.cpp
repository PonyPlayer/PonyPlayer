//
// Created by ColorsWind on 2022/3/24.
//
#include <QDateTime>
#include <QMutex>
#include <QFileInfo>
#include <iostream>
#include <random>

#include "logger.h"
#include "ponyplayer.h"

class Logger {
private:
    QTextStream qTextStream{stderr};
    QTextStream logStream;
    QMutex mutex;
    const QMap<QtMsgType, QString> types = {
            {QtDebugMsg, "D"},
            {QtWarningMsg, "W"},
            {QtInfoMsg, "I"},
            {QtFatalMsg, "F"},
            {QtCriticalMsg, "C"}
    };
    QFile logFile;
    static QString getCurrentDateTime() {
        return QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
    }
public:
    static std::string randStr(int length) {
        char tmp;
        std::string buffer;

        std::random_device rd;
        std::default_random_engine random(rd());

        for (int i = 0; i < length; i++) {
            tmp = static_cast<char>(random() % 36);
            if (tmp < 10) {
                tmp += '0';
            } else {
                tmp -= 10;
                tmp += 'A';
            }
            buffer += tmp;
        }
        return buffer;
    }

    Logger() {
        auto home = PonyPlayer::getHome();
        QDir dir(home);
        dir.mkdir("log");
        dir.cd("log");

        auto ddl = QDateTime::currentDateTime().addDays(-7).toString("yyyy-MM-dd");

        for (auto& filename : dir.entryList({"*.log"})) {
            auto birthTime = filename.mid(0, 10);
            if (birthTime < ddl) {
                dir.remove(filename);
            }
        }

        QString logFilename =  home + QString("/log/") + QDateTime::currentDateTime().toString("yyyy-MM-dd-")
                       + randStr(16).c_str() + ".log";
        logFile.setFileName(logFilename);
        logFile.open(QIODevice::ReadWrite | QIODevice::Text);
        logStream.setDevice(&logFile);
    }

    ~Logger() {
        logFile.close();
    }

    QString getLogFile() {
        return QFileInfo(logFile).absoluteFilePath();
    }

#ifdef QT_DEBUG
    inline void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker locker(&mutex);
        auto currentDateTime = getCurrentDateTime();
        qTextStream << currentDateTime << " " << types.constFind(type).value();
        logStream << currentDateTime << " " << types.constFind(type).value();
        QString function = context.function; // convert to QString
        qsizetype colon = function.indexOf(':');
        qsizetype retType = function.indexOf(' ');
        if (function.isEmpty()) {
            // pass
        } else if (colon >= 0) {
            // function in class
            qsizetype param = function.indexOf('(', colon);
            if (retType < 0 || retType > colon) {
                // no return, constructor or destructor
                qTextStream << " " << function.mid(colon + 2, param - (colon + 2));
                logStream << " " << function.mid(colon + 2, param - (colon + 2));
            } else {
                // member function
                QString clazz = function.mid(retType + 1, (colon - (retType + 1)));
                QString func = function.mid(colon + 2,  param - (colon + 2));
                qTextStream << " " << clazz << "::" << func;
                logStream << " " << clazz << "::" << func;
            }
        } else {
            // global function
            qsizetype param = function.indexOf('(');
            qTextStream << " "  << function.mid(retType + 1, param - (retType + 1));
            logStream << " "  << function.mid(retType + 1, param - (retType + 1));
        }
        qTextStream << " " << msg << "\n";
        qTextStream.flush();

        logStream << " " << msg << "\n";
        logStream.flush();
    }
#else
    inline void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker locker(&mutex);
        auto currentDateTime = getCurrentDateTime();
        qTextStream << currentDateTime<< " "<< types.constFind(type).value()<< " " << msg << "\n";
        qTextStream.flush();

        logStream << currentDateTime<< " "<< types.constFind(type).value()<< " " << msg << "\n";
        logStream.flush();
    }
#endif

};

static Logger* getLoggerInstance() {
    static Logger logger;
    return &logger;
}

void PonyPlayer::logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    getLoggerInstance()->log(type, context, msg);
}

QString PonyPlayer::getLogFile() {
    return getLoggerInstance()->getLogFile();
}