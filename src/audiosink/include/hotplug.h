#pragma once

#include <QtCore>
#include <QMediaDevices>
#include <QAudioDevice>

class PonyAudioSink;

class HotPlugDetector : public QObject {
Q_OBJECT
private:
    PonyAudioSink *audioSink;
    QMediaDevices qMediaDevices;
public:
    HotPlugDetector(PonyAudioSink *audioSink_);

signals:

};