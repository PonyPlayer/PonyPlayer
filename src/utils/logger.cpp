//
// Created by ColorsWind on 2022/3/24.
//
#include <QDateTime>
#include <QMutex>
#include <iostream>
#include <random>

#include "include/logger.h"
#include "include/ponyplayer.h"

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
    const QMap<QString, QString> classNames = {
            {"HurricanePlayer", "HPlayer"},
            {"VideoPlayWorker", "VWorker"},
            {"HurricaneRenderer", "HRender"}
    };
    const QString currentDateTime = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
    enum class State{
        INITIAL, UPPER, LOWER
    };
    QFile logFile;
public:
    QString logFilename;
#ifdef QT_DEBUG
    inline static QString getAbbrFunctionName(QString &func) {
        State state = State::INITIAL;
        QString ret;
        for(auto && c : func) {
            switch (state) {
                case State::INITIAL:
                    ret.append(c.toUpper());
                    state = State::LOWER;
                    break;
                case State::UPPER:
                    if (c.isLower()) {
                        state = State::LOWER;
                    }
                    break;
                case State::LOWER:
                    if (c.isUpper()) {
                        ret.append(".");
                        ret.append(c.toUpper());
                        state = State::UPPER;
                    }
                    break;
            }
        }
        return ret;
    }

    inline QString getClassName(const QString &clazz) const {
        auto iter = classNames.constFind(clazz);
        return iter == classNames.cend() ? clazz : iter.value();
    }

    inline void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker locker(&mutex);
        qTextStream << currentDateTime << " " << types.constFind(type).value();
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
            } else {
                // member function
                QString clazz = function.mid(retType + 1, (colon - (retType + 1)));
                QString func = function.mid(colon + 2,  param - (colon + 2));
                qTextStream << " " << getClassName(clazz) << "::" << func;
            }
        } else {
            // global function
            qsizetype param = function.indexOf('(');
            qTextStream << " "  << function.mid(retType + 1, param - (retType + 1));
        }
        qTextStream << " " << msg << "\n";
        qTextStream.flush();
    }
#else
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

        logFilename =  home + QString("/log/") + QDateTime::currentDateTime().toString("yyyy-MM-dd-")
                    + randStr(16).c_str() + ".log";
        logFile.setFileName(logFilename);
        logFile.open(QIODevice::ReadWrite | QIODevice::Text);
        logStream.setDevice(&logFile);
    }

    ~Logger() {
        logFile.close();
    }

    inline void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker locker(&mutex);
        logStream << currentDateTime<< " "<< types.constFind(type).value()<< " " << msg << "\n";
        logStream.flush();
    }
#endif

};

Logger* getLoggerInstance() {
    static Logger logger;
    return &logger;
}

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    getLoggerInstance()->log(type, context, msg);
}

QString getLogName() {
    return getLoggerInstance()->logFilename;
}