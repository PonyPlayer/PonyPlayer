//
// Created by ColorsWind on 2022/5/5.
//

#pragma once
#include <QObject>
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
        m_affinityThread = new QThread;
        m_affinityThread->setObjectName("FrameControllerThread");
        this->moveToThread(m_affinityThread);
        connect(m_affinityThread, &QThread::started, [this]{
            this->m_demuxer = new Demuxer{this};
            this->m_playback = new Playback{m_demuxer, this};
            connect(m_playback, &Playback::setPicture, this, &FrameController::setPicture, Qt::DirectConnection);
            connect(m_playback, &Playback::stateChanged, this, &FrameController::playbackStateChanged, Qt::DirectConnection);
            connect(this, &FrameController::signalDecoderOpenFile, m_demuxer, &Demuxer::openFile);
            // WARNING: BLOCKING_QUEUED_CONNECTION!!!
            connect(this, &FrameController::signalDecoderSeek, m_demuxer, &Demuxer::seek, Qt::BlockingQueuedConnection);
            connect(m_demuxer, &Demuxer::openFileResult, this, [this](bool success){
                m_demuxer->start();
                if (success) { m_playback->showFrame(); }
                emit openFileResult(success);
            });
            connect(m_playback, &Playback::resourcesEnd, this, &FrameController::resourcesEnd, Qt::DirectConnection);
            connect(this, &FrameController::signalSetTrack, [this](int i){
                qreal pos = m_playback->pos();
                m_playback->stop();
                m_demuxer->pause();
                m_demuxer->setTrack(i);
                seek(pos);
            });
            connect(this, &FrameController::signalBackward, [this]{
                qreal pos = m_playback->pos();
                m_playback->stop();
                m_demuxer->pause();
                m_demuxer->backward();
                seek(pos);
                m_demuxer->start();

            });

        });
        m_affinityThread->start();
    }

    virtual ~FrameController() {
        m_affinityThread->quit();
    }

    void setTrack(int i) {
        emit signalSetTrack(i);
    }

    PONY_THREAD_SAFE void backward() {
        emit signalBackward();
    }


    /**
     * 这个方法是线程安全的
     * @return
     */
    qreal getAudioDuration() { return m_demuxer->audioDuration(); }

    /**
     * 这个方法是线程安全的
     * @return
     */
    qreal getVideoDuration() { return m_demuxer->videoDuration(); }

    /**
     * 这个方法是线程安全的
     * @return
     */
    QStringList getTracks() { return m_demuxer->getTracks(); }

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
        m_playback->stop();

        m_demuxer->pause();  // blocking, make sure pic and sample request can be blocked
        // WARNING: must make sure everything (especially PTS) has been properly updated
        // otherwise, the video thread will be BLOCKING for a long time.
        emit signalDecoderSeek(pos); // blocking connection
        m_demuxer->flush();
        m_demuxer->start();

        // time-consuming job
        qreal startPoint = m_demuxer->getSample(true).getPTS();
        if (!m_demuxer->isRewind()) {
            // if rewinding, there is no need to skip frame. (dispatcher guarantee)
            VideoFrame pic;
            while (pic = m_demuxer->getPicture(true, true), (pic.isValid() && pic.getPTS() < pos)) {
                if (!m_demuxer->popPicture(true)) {throw std::runtime_error("Assets Fail: CANNOT pop VideoFrame."); }
                pic.free();
            }
            qDebug() << pic.getPTS() << pic.isValid();

            AudioFrame sample;
            while (sample = m_demuxer->getSample(true), (sample.isValid() && sample.getPTS() < pos)) {
                if (!m_demuxer->popSample(true)) {throw std::runtime_error("Assets Fail: CANNOT pop AudioFrame."); }
                startPoint = sample.getPTS();
            }
            qDebug() << sample.getPTS() << sample.isValid();
        }

        emit signalPositionChangedBySeek(); // block
        m_playback->setStartPoint(startPoint);
        m_playback->showFrame();

        qDebug() << "start point" << startPoint;
        qDebug() << "End seek for" << pos;
    }

signals:
    void signalDecoderOpenFile(std::string path);
    void signalDecoderSeek(qreal pos);
    void signalPositionChangedBySeek();
    void signalSetTrack(int i);
    void signalBackward();

    void openFileResult(bool success);
    void playbackStateChanged(bool isPlaying);
    void resourcesEnd();
    void setPicture(VideoFrame pic);




};

