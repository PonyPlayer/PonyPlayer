//
// Created by ColorsWind on 2022/6/3.
//

#pragma once
#include <QtCore>

#include <QFile>
#include <QGuiApplication>
#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QClipboard>
#include <utility>
#include <filesystem>
#include "platform.hpp"
constexpr const static char* ISSUE_URL = "https://github.com/SCUT-SE-ProjectMP/PonyPlayer/issues";

class CrashReporter : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString crashMessage READ getCrashMessage FINAL)
    Q_PROPERTY(QString logFile READ getLogFile FINAL)
private:
    QString crashMessage;
    QString logFile;
    QProcess process;
    [[nodiscard]] const QString &getCrashMessage() const {
        return crashMessage;
    }

    [[nodiscard]] const QString &getLogFile() const {
        return logFile;
    }

public slots:

    Q_INVOKABLE void openIssueBrowser() {
        QDesktopServices::openUrl(QUrl(ISSUE_URL));
    };

    Q_INVOKABLE void copyLogPath() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(logFile);
    };

    Q_INVOKABLE void restartApplication() {
        int ret = process.startDetached();
        QGuiApplication::exit(ret - 1);
    }

public:
    CrashReporter(QString crashMessage, const QString& logFile, const QString& program, const QStringList& arguments, QObject *parent = nullptr) : QObject(parent),
        crashMessage(std::move(crashMessage)), logFile(QFileInfo(logFile).absoluteFilePath()), process() {
        process.setProgram(program);
        process.setArguments(arguments);

    }
};