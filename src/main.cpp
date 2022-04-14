#include "logger.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <QtQuick/QQuickView>
#include "view/squircle.h"

extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <libavformat/avformat.h>

#pragma GCC diagnostic pop
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    qmlRegisterType<Squircle>("Squircle", 1, 0, "Squircle");
    qInstallMessageHandler(logMessageHandler);

    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);
    return QGuiApplication::exec();
}


