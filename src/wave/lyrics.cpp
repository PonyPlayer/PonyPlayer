#include <QtCore>
#include <QHash>
#include <fstream>
#include "lyrics.h"
#include "liblrc/lyrics.h"
#include "liblrc/lrc_parser.h"


void LyricsReader::readLyric(const QString &url) {
    std::ifstream stream(url.toStdString());
    lrc::LrcParser parser;
    std::shared_ptr<lrc::Lyrics> lyrics = parser.ParseStream(&stream);
    emit lyricReadCompleted(lyrics);
}


inline unsigned long qHash(const LyricSentence &sentence, unsigned long seed = 0) {
    seed = qHash(sentence.getStartTime(), seed);
    seed = qHash(sentence.getEndTime(), seed);
    seed = qHash(sentence.getSentence(), seed);
    return seed;
}


QQmlListProperty<LyricSentence> LyricsData::getSentences() {
    return {this, this,
            &LyricsData::appendSentence,
            &LyricsData::sentenceCount,
            &LyricsData::getSentence,
            &LyricsData::clearSentences,
            &LyricsData::replaceSentence,
            &LyricsData::removeLastSentence};

}

void LyricsData::appendSentence(QQmlListProperty<LyricSentence> *list, LyricSentence *p) {
    reinterpret_cast<LyricsData *>(list->data)->appendSentences(p);
}

qsizetype LyricsData::sentenceCount(QQmlListProperty<LyricSentence> *list) {
    return reinterpret_cast<LyricsData *>(list->data)->sentenceCount();
}

LyricSentence *LyricsData::getSentence(QQmlListProperty<LyricSentence> *list, qsizetype index) {
    return reinterpret_cast<LyricsData *>(list->data)->getSentence(index);
}

void LyricsData::clearSentences(QQmlListProperty<LyricSentence> *list) {
    return reinterpret_cast<LyricsData *>(list->data)->clearSentences();
}

void LyricsData::appendSentences(LyricSentence *p) {
    qDebug() << "APPENDED";
    m_sentences.append(p);
}

LyricSentence *LyricsData::getSentence(qsizetype index) const {
    return m_sentences.at(index);
}

void LyricsData::replaceSentence(qsizetype index, LyricSentence *p) {
    m_sentences[index] = p;
}

void LyricsData::removeLastSentence() {
    m_sentences.pop_back();
}

void LyricsData::replaceSentence(QQmlListProperty<LyricSentence> *list, qsizetype index, LyricSentence *p) {
    reinterpret_cast<LyricsData *>(list->data)->replaceSentence(index, p);
}

qsizetype LyricsData::sentenceCount() const {
    return m_sentences.size();
}

void LyricsData::clearSentences() {
    m_sentences.clear();
}

void LyricsData::removeLastSentence(QQmlListProperty<LyricSentence> *list) {
    reinterpret_cast<LyricsData *>(list->data)->removeLastSentence();
}

LyricsData::LyricsData() {
}

