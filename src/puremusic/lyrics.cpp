#include <QtCore>
#include <QHash>
#include <fstream>
#include "lyrics.h"
#include "liblrc/lyrics.h"
#include "liblrc/lrc_parser.h"


std::unique_ptr<lrc::Lyrics> LyricReader::readLyric(const QString &filePath) {
    std::ifstream stream(filePath.toStdString());
    lrc::LrcParser parser;
    std::unique_ptr<lrc::Lyrics> lyrics = parser.ParseStream(&stream);
    return lyrics;
}


inline unsigned long qHash(const LyricSentence &sentence, unsigned long seed = 0) {
    seed = qHash(sentence.getTime(), seed);
    seed = qHash(sentence.getSentence(), seed);
    return seed;
}

void Lyric::addSentence(const LyricSentence &sentence) {
    content.insert(sentence);
}

