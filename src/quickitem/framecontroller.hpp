//
// Created by ColorsWind on 2022/5/5.
//

#pragma once
#ifndef PONYPLAYER_FRAMECONTROLLER_HPP
#define PONYPLAYER_FRAMECONTROLLER_HPP
#include <QObject>
#include "demuxer2.h"
#include "playback.hpp"

/**
 * @brief 将解码拿到的帧传递给输出.
 *
 *
 */
class FrameController: public QObject {
    Q_OBJECT
private:
    QThread *m_affinityThread;
    Demuxer *m_demuxer;
    Playback *m_playback;
public:
    FrameController(QObject *parent): QObject(nullptr) {
        m_affinityThread = new QThread{parent};
        m_affinityThread->setObjectName("FrameControllerThread");
        this->moveToThread(m_affinityThread);
        connect(m_affinityThread, &QThread::started, [=]{
            this->m_demuxer = new Demuxer{this};
            this->m_playback = new Playback{m_demuxer, this};
            connect(m_playback, &Playback::setPicture, this, &FrameController::setPicture);
            connect(m_playback, &Playback::stateChanged, this, &FrameController::playbackStateChanged);
            connect(this, &FrameController::signalDecoderOpenFile, m_demuxer, &Demuxer::openFile);
            connect(this, &FrameController::signalDecoderSeek, m_demuxer, &Demuxer::seek);
            connect(m_demuxer, &Demuxer::openFileResult, this, &FrameController::openFileResult);
            connect(m_playback, &Playback::resourcesEnd, this, &FrameController::resourcesEnd);
        });
        m_affinityThread->start();
    }

    virtual ~FrameController() {
        m_affinityThread->quit();
//        m_affinityThread->deleteLater();
        m_playback->stop();
        m_playback->deleteLater();
        m_demuxer->close();
        m_demuxer->deleteLater();
    }

    qreal getAudioDuration() { return m_demuxer->audioDuration(); }
    qreal getVideoDuration() { return m_demuxer->videoDuration(); }

    void setVolume(qreal volume) {m_playback->setVolume(volume); }
    void setSpeed(qreal speed) {m_playback->setSpeed(speed); }


public slots:
    void openFile(const QString &path) {
        QUrl url(path);
        QString localPath = url.toLocalFile();
        qDebug() << "Open file" << localPath;
        emit signalDecoderOpenFile(localPath.toStdString());
    }

    void pause() {
        qDebug() << "Pausing";
        m_playback->pause();
    }

    void stop() {
        qDebug() << "Stopping";
        m_demuxer->close();
        m_demuxer->flush();
        m_playback->stop();
    }

    void close() {
        qDebug() << "Closing";
        m_demuxer->close();
        m_playback->stop();
    }

    void start() {
        qDebug() << "Starting";
        m_demuxer->start();
        m_playback->start();
    }

    void seek(qreal pos) {
        qDebug() << "Start seek for" << pos;
        bool isPlaying = m_playback->isPlaying();
        m_playback->stop();

        // WARNING: must make sure everything (especially PTS) has been properly updated
        // otherwise, the video thread will be BLOCKING for a long time.
        emit signalDecoderSeek(static_cast<qreal>(pos * 1000 * 1000)); // blocking connection
        m_demuxer->flush();
        m_demuxer->start(); // blocking, make sure pic and sample request can be blocked

        // time-consuming job
        {
            Picture pic;
            while(pic = m_demuxer->getPicture(true), (pic.isValid() && pic.getPTS() < pos)) {
                m_demuxer->popPicture(true);
                pic.free();
            }
        }
        qreal startPoint = m_demuxer->getSample(true).getPTS();
        {

            Sample sample;
            while(sample = m_demuxer->getSample(true), (sample.isValid() && sample.getPTS() < pos)) {
                m_demuxer->popSample(true);
                startPoint = sample.getPTS();
                sample.free();
            }
            qDebug() << sample.getPTS() << sample.isValid();

        }
        m_playback->clear();
        emit signalPositionChangedBySeek();
        m_playback->setStartPoint(startPoint);
        qDebug() << "start point" << startPoint;
        if (isPlaying) {
            m_playback->start();
        }
        qDebug() << "End seek for" << pos;
    }

signals:
    void signalDecoderOpenFile(std::string path);
    void signalDecoderSeek(qreal pos);
    void signalPositionChangedBySeek();


    void openFileResult(bool success);
    void playbackStateChanged(bool isPlaying);
    void resourcesEnd();
    void setPicture(Picture pic);

};

#endif //PONYPLAYER_FRAMECONTROLLER_HPP
