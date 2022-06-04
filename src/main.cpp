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
#include "lyrics.h"
#include "crashreporter.hpp"
#include <csignal>


namespace PonyPlayer {
    QStringList programArguments;

    int reportErrorMain(int argc, char *argv[]) {
        QString program = argv[0];
        QString logFile;
        QString message;
        QStringList arguments;
        for(int i=1;i<argc;) {
            auto && argument = argv[i];
            if (strcmp(argument, "--crash--report") == 0) {
                for(int j=1;j<i;++j) { arguments.emplace_back(argument); }
            } else if (strcmp(argument, "--log-file") == 0 && i + 1 < argc) {
                logFile = argv[i + 1];
                i += 2;
            } else if (strcmp(argument, "--message") == 0 && i + 1 < argc) {
                message = argv[i + 1];
                i += 2;
            } else {
                i += 1;
            }
        }
        QGuiApplication app(argc, argv);
        const QUrl url(u"qrc:/view/IssueWindow.qml"_qs);
        QQmlApplicationEngine engine;
        CrashReporter crashReporter(message, logFile, program, arguments);
        engine.rootContext()->setContextProperty("crash_reporter", &crashReporter);
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
                    if (!obj && url == objUrl)
                        QCoreApplication::exit(-1);
                }, Qt::QueuedConnection);
        engine.load(url);
        return QGuiApplication::exec();
    }

    void startReporterProcess(const std::string& message) {
        QString &program = programArguments.front();
        QStringList arguments;
        for(int i=1;i<programArguments.size();++i) {
            arguments.emplace_back(programArguments[i]);
        }
        arguments.append("--crash-report");
        arguments.append("--log-file");
        arguments.append(PonyPlayer::getLogFile());
        arguments.append("--message");
        arguments.append(message.c_str());
        QProcess process(nullptr);
        process.setProgram(program);
        process.setArguments(arguments);
        qint64 pid;
        process.startDetached(&pid);
    }

    void signalHandler( int signum ) {
        PonyPlayer::startReporterProcess("SIGNAL: " + std::to_string(signum));
        exit(signum);
    }
}




int main(int argc, char *argv[]) {
    for(int i=0;i<argc;++i) {
        if (strcmp(argv[i], "--crash-report") == 0) {
            return PonyPlayer::reportErrorMain(argc, argv);
        }
        PonyPlayer::programArguments.append(argv[i]);
    }

    signal(SIGABRT, PonyPlayer::signalHandler);
    signal(SIGSEGV, PonyPlayer::signalHandler);

    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
//    format.setSamples(16);
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
