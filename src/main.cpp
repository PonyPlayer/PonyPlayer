#include <logger.h>
#include "config.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include "playlist.h"

void hello() {
//    (new Demuxer())->videoFrameQueueFront();
}

#include "hurricane.h"
#include "kv_engine.h"
#include "config.h"

extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <libavformat/avformat.h>

#pragma GCC diagnostic pop
}

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    qmlRegisterType<Hurricane>("Hurricane", 1, 0, "Hurricane");
    qInstallMessageHandler(logMessageHandler);

    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);




    PlayList playList("testList");

    return QApplication::exec();
}
