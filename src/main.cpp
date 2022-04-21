
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include "hurricane.h"
#include "logger.h"
#include "quickitem.h"

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    qmlRegisterType<HurricanePlayer>("HurricanePlayer", 1, 0, "HurricanePlayer");
    qInstallMessageHandler(logMessageHandler);
    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);
    return QApplication::exec();
}
