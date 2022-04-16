#include <logger.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

void hello()
{
//    (new Demuxer())->videoFrameQueueFront();
}
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qInstallMessageHandler(logMessageHandler);

    const QUrl url(u"qrc:/view/main.qml"_qs);
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    hello();
    return QGuiApplication::exec();
}


