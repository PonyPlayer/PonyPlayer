//
// Created by ColorsWind on 2022/3/24.
//
#include <QDateTime>
#include <QMutex>
#include <QVector>

#include "logger.h"

class Logger {
private:
    QTextStream qTextStream{stderr};
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
public:
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
                qTextStream << " " << getClassName(clazz) << "::" << getAbbrFunctionName(func);
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
    inline void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker locker(&mutex);
        qTextStream << currentDateTime << " "<< types.constFind(type).value() << " " << msg << "\n";
        qTextStream.flush();
    }
#endif

};



void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static Logger logger;

    logger.log(type, context, msg);
}