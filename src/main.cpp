#include <logger.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "hurricane.h"
#include "demuxer.h"

void hello() {
    Demuxer demuxer;
    demuxer.initDemuxer();
    av_frame_new_side_data_from_buf(demuxer.videoFrameQueueFront()->frame, static_cast<AVFrameSideDataType>(0), 0);
}

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
    QGuiApplication app(argc, argv);
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
    return QGuiApplication::exec();
}
