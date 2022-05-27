#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include <QQmlContext>
#include "utils/include/logger.h"
#include "players.h"
#include "controller.h"
#include "playlist.h"
#include "wave/wave.hpp"
#include "hotloader.hpp"
#include "cursor.hpp"
#include "include/lyrics.h"
#include "liblrc/lyrics.h"
#include "liblrc/lrc_parser.h"

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
//    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    QSurfaceFormat::setDefaultFormat(format);
    QGuiApplication app(argc, argv);

//    qmlRegisterType<Hurricane>("HurricanePlayer", 1, 0, "HurricanePlayer");
    registerPlayerQML();
    qmlRegisterType<WaveView>("WaveView", 1, 0, "WaveView");
    qmlRegisterType<Controller>("Controller", 1, 0, "Controller");
    qmlRegisterType<PlayList>("PlayList", 1, 0, "PlayList");
    qmlRegisterType<simpleListItem>("SimpleListItem", 1, 0, "SimpleListItem");
    qmlRegisterType<LyricsData>("LyricsData", 1, 0, "LyricsData");
    qmlRegisterType<LyricSentence>("LyricSentence", 1, 0, "LyricSentence");
    qRegisterMetaType<PlayListItem *>("PlayListItem");
    qmlRegisterUncreatableMetaObject(
            PonyPlayer::staticMetaObject, // meta object created by Q_NAMESPACE macro
            "ponyplayer.ns",                // import statement (can be any string)
            1, 0,                          // major and minor version of the import
            "PonyPlayerNS",                 // name in QML (does not have to match C++ name)
            "Error: only enums"            // error in case someone tries to create a MyNamespace object
    );
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



    return QGuiApplication::exec();
}
