#pragma once

#include <QtCore>
#include <QtQuick/QQuickItem>
#include <QQmlListProperty>
#include <utility>
#include "liblrc/lyrics.h"

class LyricSentence : public QObject {
Q_OBJECT
    Q_PROPERTY(QTime startTime READ getStartTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QTime endTime READ getEndTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(QString sentence READ getSentence WRITE setSentence NOTIFY sentenceChanged)
    QML_ELEMENT
private:
    QTime startTime, endTime;
    QString sentence;
public:
    Q_INVOKABLE LyricSentence() = default;

    Q_INVOKABLE explicit LyricSentence(QTime startTime, QTime endTime, QString sentence) :
            startTime(startTime), endTime(endTime), sentence(std::move(sentence)) {}

    Q_INVOKABLE bool operator<(const LyricSentence &t) const { return startTime < t.startTime; }

    Q_INVOKABLE bool operator==(const LyricSentence &t) const {
        return startTime == t.startTime && endTime == t.endTime && sentence == t.sentence;
    }

    Q_INVOKABLE QTime getStartTime() const { return startTime; }

    Q_INVOKABLE QTime getEndTime() const { return endTime; }

    Q_INVOKABLE QString getSentence() const { return sentence; }

    Q_INVOKABLE void setStartTime(const QTime &newTime) { startTime = newTime; }

    Q_INVOKABLE void setEndTime(const QTime &newTime) { endTime = newTime; }

    Q_INVOKABLE void setSentence(const QString &newSentence) { sentence = newSentence; }

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

    void pushLyricSentence(LyricSentence *sentence) {
        m_sentences.push_back(sentence);
    };
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