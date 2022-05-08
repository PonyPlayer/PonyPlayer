//
// Created by ColorsWind on 2022/5/8.
//

#pragma once
#include <QObject>
#include <QImage>
#include <QThread>
#include "previewer.hpp"

class Preview : public QObject {
    Q_OBJECT
private:
    Previewer *m_worker = nullptr;
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

    ~Preview() override {
        // set interrupt flag
        m_affinityThread->quit();
    }
public slots:
    void previewRequest(qreal pos) {
        QImage res;
        if (m_worker) {
            res = m_worker->previewRequest(pos);
        }
        emit previewResult(pos, res);
    };

    void openFile(const std::string &fn) {
        qDebug() << "Previewer: Open file" << QString::fromUtf8(fn);
        if (m_worker) {
            qWarning() << "Previewer: Already open file:" << m_worker->filename.c_str();
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        try {
            m_worker = new Previewer(fn, this);
        } catch (std::runtime_error &ex) {
            qWarning() << "Previewer: Error opening file:" << ex.what();
            m_worker = nullptr;
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        emit openFileResult(true, QPrivateSignal());
    }

    void close() {
        if (m_worker) {
            qDebug() << "Previewer: Close file" << m_worker->filename.c_str();
            m_worker->deleteLater();
            m_worker = nullptr;
        } else {
            qWarning() << "Previewer: Try to close file while no file has been opened.";
        }
    }

signals:
    void previewResult(qreal pos, QImage image);

    void openFileResult(bool ret, QPrivateSignal);
};