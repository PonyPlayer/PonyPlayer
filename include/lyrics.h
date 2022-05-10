#include <QtCore>
#include <utility>
#include "liblrc/lyrics.h"

class LyricSentence {
private:
    QTime time;
    QString sentence;
public:
    explicit LyricSentence(QTime time, QString sentence) : time(time), sentence(std::move(sentence)) {}

    bool operator<(const LyricSentence &t) const { return time < t.time; }

    bool operator==(const LyricSentence &t) const { return time == t.time && sentence == t.sentence; }

    QTime getTime() const { return time; }

    QString getSentence() const { return sentence; }
};

class Lyric {
private:
    QSet<LyricSentence> content;
public:
    Lyric() = default;

    void addSentence(const LyricSentence &sentence);
};

class LyricReader : public QObject {
Q_OBJECT
public:
    LyricReader() = default;

    static std::unique_ptr<lrc::Lyrics> readLyric(const QString &filePath);
};