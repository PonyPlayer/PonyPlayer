//
// Created by ColorsWind on 2022/4/21.
//

#ifndef PONYPLAYER_QUICKITEM_H
#define PONYPLAYER_QUICKITEM_H

#include <QRunnable>
#include <QAtomicInt>
#include <QAudioSink>
#include <QThread>
#include <chrono>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "hurricane.h"
#include "demuxer.h"

class HurricanePlayer;

using std::chrono::system_clock;

class HurricaneState : public QObject {
Q_OBJECT
    QML_ELEMENT
public:

};

class VideoPlayWorker : public QObject {
    Q_OBJECT
private:
    Demuxer *demuxer;
    QAudioSink audioSink;
    Hurricane *hurricane;
    QAtomicInteger<bool> pauseRequested = false;
    QAudioSink *audioOutput;
public:
    VideoPlayWorker(Demuxer *d, Hurricane *h) : QObject(nullptr), demuxer(d), hurricane(h) {
        QAudioFormat format;
        format.setSampleRate(44100);
        format.setChannelCount(2);
        format.setSampleFormat(QAudioFormat::Int16);
        audioOutput = new QAudioSink(format);
        audioOutput->setVolume(100);
        audioOutput->setBufferSize(192000 * 10);
    }
    void resume() { pauseRequested = false; }
    void pause() { pauseRequested = true; }

    unsigned long cast(long ms) {
        if (ms < 0) return 0L;
        return static_cast<unsigned long>(ms);
    }
public slots:
    void onWork() {
        qDebug() << "Start Video Work";
        auto *audioInput = audioOutput->start();
        Picture currFrame;
        while(!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
            demuxer->popPicture();
            emit setImage(currFrame);
            Picture nextFrame = demuxer->getPicture(true);
            while(audioOutput->bytesFree() >  192000) {
                Sample sample = demuxer->getSample(true);
                audioInput->write(reinterpret_cast<const char *>(sample.data), sample.len);
                demuxer->popSample();
            }
            if (nextFrame.isValid()) {
                QThread::msleep(cast(static_cast<long>(nextFrame.pts * 1000) - audioOutput->processedUSecs() / 1000));
            }
        }
        audioOutput->stop();
        emit videoInterrupted();
        qDebug() << "end";
    }
signals:
    void setImage(Picture pic);
    void videoInterrupted();
};

class HurricanePlayer : public Hurricane {
    Q_OBJECT
    QML_ELEMENT
//    Q_PROPERTY(HurricaneState state NOTIFY stateChanged)
private:
    Demuxer demuxer;
    QThread *videoThread;
    VideoPlayWorker videoPlayWorker;
public:
    HurricaneState state;

    HurricanePlayer(QQuickItem *parent = nullptr);

    virtual ~HurricanePlayer();

signals:

    /**
     * 状态发生改变
     */
    void stateChanged();

    /**
     * 视频播放完
     */
    void playFinished();

    void videoStart();
    void videoPause();
    void taskStop();

public slots:

    /**
     * 打开视频文件
     * @param path
     */
    Q_INVOKABLE void openFile(const QString &path);

    /**
     * 开始播放视频
     */
    Q_INVOKABLE void start();

    /**
     * 停止播放视频
     */
    Q_INVOKABLE void stop();

    /**
     * 设置视频倍速
     */
    Q_INVOKABLE void setSpeed(qreal speed);


    /**
     * 暂停播放视频
     */
    Q_INVOKABLE void pause();

    /**
     * 将视频播放进度移动到指定位置
     * 注：这个操作不会改变视频的播放状态
     */
    Q_INVOKABLE void seek(qreal sec);
};


#endif //PONYPLAYER_QUICKITEM_H
