#pragma once

#include <QtCore>
#include <QMediaDevices>
#include <QAudioDevice>

class HotPlugDetector : public QObject {
    Q_OBJECT
private:
    QMediaDevices qMediaDevices;
public:
    HotPlugDetector(QObject *parent = nullptr) : QObject(parent)  {
        connect(&qMediaDevices, &QMediaDevices::audioOutputsChanged, this, &HotPlugDetector::audioOutputsChanged);
    }

signals:
    void audioOutputsChanged();
};