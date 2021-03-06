#pragma once

#include <QtCore>
#include "lyrics.h"
#include "hurricane.hpp"
#include <QFileInfo>
#include <QQmlContext>
#include <QtQuick/QQuickItem>
#include <QQmlApplicationEngine>

class WaveView : public QQuickItem {
Q_OBJECT
    Q_PROPERTY(float radiusRatio READ getRadiusRatio WRITE setRadiusRatio NOTIFY notifyRadiusRatio)
    Q_PROPERTY(QString player READ getPlayer WRITE setPlayer)
    Q_PROPERTY(QString lyricsDataName READ getLyricsDataName WRITE setLyricsDataName)
    QML_ELEMENT
private:
    float m_radiusRatio{};
    QString m_player, m_lyricsDataName;
    Hurricane *hurricane = nullptr;
    LyricsReader *lyricsReader = nullptr;
    LyricsData *lyricsData = nullptr;

public:
    explicit WaveView(QQuickItem *parent = nullptr) {
        lyricsReader = new LyricsReader;

        connect(this, &QQuickItem::windowChanged, [this](QQuickWindow *window) {
            if (!window) { return; }
            auto *context = qmlContext(this);
            while ((!hurricane || !lyricsData) && context) {
                if (!hurricane) hurricane = reinterpret_cast<Hurricane *>(context->objectForName(m_player));
                if (!lyricsData) lyricsData = reinterpret_cast<LyricsData *>(context->objectForName(m_lyricsDataName));
                context = context->parentContext();
            }
            if (!hurricane) {
                throw std::runtime_error("Cannot not get Hurricane by id:" + m_player.toStdString());
            }
            if (!lyricsData) {
                throw std::runtime_error("Cannot not get LyricsData by id:" + m_lyricsDataName.toStdString());
            }
            //connect(hurricane, &Hurricane::signalOpenFile, lyricsReader, &LyricsReader::readLyric);
            connect(lyricsReader, &LyricsReader::lyricReadCompleted, this, &WaveView::readLyricsResponse);
            connect(this, &WaveView::signalPushLyricSentence, lyricsData, &LyricsData::pushLyricSentence);
            connect(this, &WaveView::signalPushLyricSentenceCompleted, lyricsData,
                    &LyricsData::pushLyricSentenceCompleted);
            connect(this, &WaveView::signalClearLyricSentence, lyricsData,
                    &LyricsData::clearLyricSentence);

        });
    };

    Q_INVOKABLE [[nodiscard]] float getRadiusRatio() const { return m_radiusRatio; };

    [[nodiscard]] const QString &getPlayer() const {
        return m_player;
    }

    void setPlayer(const QString &player) {
        m_player = player;
        qDebug() << "Set" << player;
    }

    [[nodiscard]] const QString &getLyricsDataName() const {
        return m_lyricsDataName;
    }

    void setLyricsDataName(const QString &lyricsDataName) {
        m_lyricsDataName = lyricsDataName;
        qDebug() << "Set" << lyricsDataName;
    }

public slots:
    Q_INVOKABLE void setRadiusRatio(float radiusRatio) {
        m_radiusRatio = radiusRatio; emit notifyRadiusRatio();
    };

    Q_INVOKABLE void readLyricsResponse(std::shared_ptr<lrc::Lyrics> lyrics) {
        for (auto it = lyrics->IteratorBegin(); it != lyrics->IteratorEnd(); it++) {
            emit signalPushLyricSentence(
                    new LyricSentence(static_cast<double>(it->start_time) / 1000,
                                      static_cast<double>(it->end_time) / 1000,
                                      QString(it->lyric.data())));

        }
        emit signalPushLyricSentenceCompleted();
    }

    Q_INVOKABLE bool readLyrics(const QString &filePath) {
        lyricsReader->readLyric(filePath);
        return true;
    }

    Q_INVOKABLE bool tryLoadLyrics(const QString &filePath) {
        emit signalClearLyricSentence();
        QUrl url(filePath);
        QString lrcPath =
                QFileInfo(url.toLocalFile()).dir().path() + "/" + QFileInfo(url.toLocalFile()).completeBaseName() +
                ".lrc";
        if (QFile(lrcPath).exists()) readLyrics(lrcPath);
        return true;
    }

signals:

    void signalPushLyricSentence(LyricSentence *lyricSentence);

    void signalClearLyricSentence();

    void notifyRadiusRatio();

    void signalPushLyricSentenceCompleted();

};