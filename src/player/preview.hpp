//
// Created by ColorsWind on 2022/5/8.
//

#pragma once
#include <QObject>
#include <QImage>
#include <QThread>
class Preview : public QObject {
    Q_OBJECT
private:
    QThread *m_affinityThread;
public:
    Preview(QObject *parent) {
        m_affinityThread = new QThread;
        m_affinityThread->setObjectName("PreviewThread");
        this->moveToThread(m_affinityThread);
        connect(m_affinityThread, &QThread::started, this, [=]{
           // init here
        });
        m_affinityThread->start();
    }

    ~Preview() {
        // set interrupt flag
        m_affinityThread->quit();
    }
public slots:
    void previewRequest(qreal pos) {
        emit previewResult(pos, QImage());
    };
signals:
    void previewResult(qreal pos, QImage image);
};