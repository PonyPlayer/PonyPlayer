#pragma once

#include <QtCore>
#include <QtQuick/QQuickItem>
#include <QQmlListProperty>
#include <utility>
#include "liblrc/lyrics.h"

class LyricSentence : public QObject {
Q_OBJECT
    Q_PROPERTY(qreal startTime READ getStartTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(qreal endTime READ getEndTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(QString sentence READ getSentence WRITE setSentence NOTIFY sentenceChanged)
    QML_ELEMENT
private:
    qreal m_startTime, m_endTime;
    QString m_sentence;
public:
    Q_INVOKABLE LyricSentence() = default;

    Q_INVOKABLE explicit LyricSentence(qreal startTime, qreal endTime, QString sentence) :
            m_startTime(startTime), m_endTime(endTime), m_sentence(std::move(sentence)) {}

    Q_INVOKABLE bool operator<(const LyricSentence &t) const { return m_startTime < t.m_startTime; }

    Q_INVOKABLE bool operator==(const LyricSentence &t) const {
        return m_startTime == t.m_startTime && m_endTime == t.m_endTime && m_sentence == t.m_sentence;
    }

    Q_INVOKABLE qreal getStartTime() const { return m_startTime; }

    Q_INVOKABLE qreal getEndTime() const { return m_endTime; }

    Q_INVOKABLE QString getSentence() const { return m_sentence; }

    Q_INVOKABLE void setStartTime(const qreal &newTime) { m_startTime = newTime; }

    Q_INVOKABLE void setEndTime(const qreal &newTime) { m_endTime = newTime; }

    Q_INVOKABLE void setSentence(const QString &newSentence) { m_sentence = newSentence; }

signals:

    void startTimeChanged();

    void endTimeChanged();

    void sentenceChanged();

};

class LyricsData : public QObject {
Q_OBJECT
    Q_PROPERTY(QQmlListProperty<LyricSentence> sentences READ getSentences NOTIFY lyricsChange)
    QML_ELEMENT
private:

    QVector<LyricSentence *> m_sentences;

    static void appendSentence(QQmlListProperty<LyricSentence> *, LyricSentence *);

    static qsizetype sentenceCount(QQmlListProperty<LyricSentence> *);

    static LyricSentence *getSentence(QQmlListProperty<LyricSentence> *, qsizetype);

    static void clearSentences(QQmlListProperty<LyricSentence> *);

    static void replaceSentence(QQmlListProperty<LyricSentence> *, qsizetype, LyricSentence *);

    static void removeLastSentence(QQmlListProperty<LyricSentence> *);

public:
    LyricsData();

    QQmlListProperty<LyricSentence> getSentences();

    void appendSentences(LyricSentence *);

    [[nodiscard]] qsizetype sentenceCount() const;

    [[nodiscard]] LyricSentence *getSentence(qsizetype) const;

    void clearSentences();

    void replaceSentence(qsizetype, LyricSentence *);

    void removeLastSentence();

signals:

    void lyricsChange();

public slots:

    void clearLyricSentence() { m_sentences.clear(); emit lyricsChange();};

    void pushLyricSentence(LyricSentence *sentence) {
        m_sentences.push_back(sentence);
        qDebug() << sentence->getStartTime();
    };

    void pushLyricSentenceCompleted() {
        emit lyricsChange();
    }
};

class LyricsReader : public QObject {
Q_OBJECT
public:
    LyricsReader() = default;

public slots:

    void readLyric(const QString &url);

signals:

    void lyricReadCompleted(std::shared_ptr<lrc::Lyrics> response);

};