//
// Created by ColorsWind on 2022/5/2.
//

#ifndef PONYPLAYER_HOTLOADER_H
#define PONYPLAYER_HOTLOADER_H
#include <QtCore>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <filesystem>

/**
 * @brief 提供热重载支持
 *
 * Debug模式下按 F5 进行热重载
 */
class HotLoader : public QObject {
    Q_OBJECT
private:
    QQmlApplicationEngine *engine;
public:
    explicit HotLoader(QQmlApplicationEngine *e): engine(e) {
        qDebug() << "Construct HotLoader.";
    }

    Q_INVOKABLE void reload() {
#ifdef QT_DEBUG
        static std::string mainQML = std::filesystem::path(__FILE__).parent_path().string() + "/view/main.qml";

        auto *rootObject = engine->rootObjects().first();
        auto* mainWindow = qobject_cast<QQuickWindow*>(rootObject);
        Q_ASSERT( mainWindow != nullptr );
        mainWindow->close();
        engine->clearComponentCache();
        mainWindow->deleteLater();
        engine->load(QUrl::fromLocalFile(QString::fromUtf8(mainQML)));
        qWarning() << "Complete hot reloading.";
#endif
    }
};

#endif //PONYPLAYER_HOTLOADER_H
