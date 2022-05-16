#pragma once

#include <QtCore>
#include "lyrics.h"
#include "hurricane.hpp"
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
    QString player, lyricsDataName;
    Hurricane *hurricane = nullptr;
    LyricsReader *lyricsReader = nullptr;
    LyricsData *lyricsData = nullptr;

public:
    explicit WaveView(QQuickItem *parent = nullptr) {
        lyricsReader = new LyricsReader;

        connect(this, &QQuickItem::windowChanged, [this, parent](QQuickWindow *window) {
            if (!window) { return; }
            auto *context = qmlContext(this);
            while ((!hurricane || !lyricsData) && context) {
                if (!hurricane) hurricane = reinterpret_cast<Hurricane *>(context->objectForName(player));
                if (!lyricsData) lyricsData = reinterpret_cast<LyricsData *>(context->objectForName(lyricsDataName));
                context = context->parentContext();
            }
            if (!hurricane) {
                throw std::runtime_error("Cannot not get Hurricane by id:" + player.toStdString());
            }
            if (!lyricsData) {
                throw std::runtime_error("Cannot not get LyricsData by id:" + lyricsDataName.toStdString());
            }
            connect(hurricane, &Hurricane::signalOpenFile, lyricsReader, &LyricsReader::readLyric);
            connect(lyricsReader, &LyricsReader::lyricReadCompleted, this, &WaveView::readLyricsResponse);
            connect(this, &WaveView::signalPushLyricSentence, lyricsData, &LyricsData::pushLyricSentence);
            connect(this, &WaveView::signalPushLyricSentenceCompleted, lyricsData,
                    &LyricsData::pushLyricSentenceCompleted);

        });
    };

    Q_INVOKABLE [[nodiscard]] float getRadiusRatio() const { return m_radiusRatio; };

    [[nodiscard]] const QString &getPlayer() const {
        return player;
    }

    void setPlayer(const QString &player) {
        this->player = player;
        qDebug() << "Set" << player;
    }

    [[nodiscard]] const QString &getLyricsDataName() const {
        return lyricsDataName;
    }

    void setLyricsDataName(const QString &lyricsDataName) {
        this->lyricsDataName = lyricsDataName;
        qDebug() << "Set" << lyricsDataName;
    }

public slots:
    Q_INVOKABLE void setRadiusRatio(float radiusRatio) {
        m_radiusRatio = radiusRatio; emit notifyRadiusRatio();
    };

    Q_INVOKABLE void readLyricsResponse(std::shared_ptr<lrc::Lyrics> lyrics) {
        for (auto it = lyrics->IteratorBegin(); it != lyrics->IteratorEnd(); it++) {
            emit signalPushLyricSentence(
                    new LyricSentence(QTime(0, 0, 0, it->start_time), QTime(0, 0, 0, it->end_time),
                                      QString(it->lyric.data())));

        }
        emit signalPushLyricSentenceCompleted();
    }

    Q_INVOKABLE bool readLyrics(const QString &filePath) {
        lyricsReader->readLyric(filePath);
        return true;
    }

signals:

    void signalPushLyricSentence(LyricSentence *lyricSentence);

    void notifyRadiusRatio();

    void signalPushLyricSentenceCompleted();

};