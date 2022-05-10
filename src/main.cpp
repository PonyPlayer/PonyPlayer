#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include "utils/include/logger.h"
#include "players.h"
#include "controller.h"
#include "playlist.h"
#include "wave.h"
#include "hotloader.hpp"
#include "lyrics.h"
#include "liblrc/lyrics.h"
#include "liblrc/lrc_parser.h"

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);

//    qmlRegisterType<Hurricane>("HurricanePlayer", 1, 0, "HurricanePlayer");
    registerPlayerQML();
    qmlRegisterType<WaveView>("WaveView", 1, 0, "WaveView");
    qmlRegisterType<Controller>("Controller",1,0,"Controller");
    qmlRegisterType<PlayList>("PlayList",1,0,"PlayList");
    qmlRegisterType<simpleListItem>("SimpleListItem",1,0,"SimpleListItem");
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
    LyricReader lyricReader;
    std::unique_ptr<lrc::Lyrics> lyrics = LyricReader::readLyric("/home/gns/Fire on Fire - Sam Smith.lrc");
    for (auto it = lyrics->IteratorBegin(); it != lyrics->IteratorEnd(); it++) {
        qDebug() << it->lyric.data();
    }
    return QApplication::exec();
}
