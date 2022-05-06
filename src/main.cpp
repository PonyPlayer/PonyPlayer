
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include "fireworks.h"
#include "logger.h"

#include "controller.h"
#include "wave.h"
#include "hotloader.hpp"
#include "hurricane.hpp"

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    qmlRegisterType<Hurricane>("HurricanePlayer", 1, 0, "HurricanePlayer");
    qmlRegisterType<WaveView>("WaveView", 1, 0, "WaveView");
    qmlRegisterType<Controller>("Controller",1,0,"Controller");
    qRegisterMetaType<PlayListItem *>("PlayListItem");
    qInstallMessageHandler(logMessageHandler);

    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;
    HotLoader hotLoader(&engine);
    engine.rootContext()->setContextProperty("hotLoader", &hotLoader);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);


    qDebug() << "Start program";
    return QApplication::exec();
}
