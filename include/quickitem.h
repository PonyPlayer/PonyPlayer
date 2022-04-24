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
    QAtomicInteger<bool> pauseRequested = false;
    QAudioSink *audioOutput;
    QIODevice *audioInput;
    int64_t videoStartPoint = 0;
    int64_t elapsedUSecsOnStart = 0;

    int64_t idleStartTime = 0;
    int64_t idleTimeTotal = 0;

public:
    VideoPlayWorker(Demuxer *d) : QObject(nullptr), demuxer(d) {
    }
    void resume() { pauseRequested = false; }
    void pause() { pauseRequested = true; }




private:
    inline int64_t getProcessedAudioUSecs() {
        return audioOutput->elapsedUSecs() - idleTimeTotal;
    }
    inline void syncTo(double pts) {
        auto target = static_cast<int64_t>(pts * 1000 * 1000); // us
        auto current = getProcessedAudioUSecs() + videoStartPoint; // us
        auto duration = target - current;
        if (duration > 0) {
            QThread::usleep(static_cast<unsigned long>(duration));
        } else {
            qWarning() << "Sleep negative duration" << duration << "us";
        }
    }
public slots:
    void onChanged(QAudio::State state) {
        auto curr = audioOutput->elapsedUSecs();
        qDebug() << "QAudioSink State changed" << state;
        switch(state) {
            case QAudio::ActiveState:
                idleTimeTotal += curr -  idleStartTime;
                idleStartTime = -1;
                break;
            case QAudio::SuspendedState:
            case QAudio::StoppedState:
            case QAudio::IdleState:
                // FIXME take state transition diagram into consideration
                if (idleStartTime <= 0)
                    idleStartTime = curr;
                break;
        }
        qDebug() << idleTimeTotal;
    }
    void initOnThread() {
        QAudioFormat format;
        format.setSampleRate(44100);
        format.setChannelCount(2);
        format.setSampleFormat(QAudioFormat::Int16);
        audioOutput = new QAudioSink(format, this);
        audioOutput->setVolume(100);
        audioOutput->setBufferSize(192000 * 10);
        audioInput = audioOutput->start();
        audioOutput->suspend();
        connect(audioOutput, &QAudioSink::stateChanged, this, &VideoPlayWorker::onChanged, Qt::ConnectionType::DirectConnection);
    };
    void onWork() {
        qDebug() << "Start Video Work";
        audioOutput->resume();
        Picture currFrame;
        elapsedUSecsOnStart = audioOutput->elapsedUSecs();
        while(!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
            demuxer->popPicture();
            emit setImage(currFrame);
            Picture nextFrame = demuxer->getPicture(true);
            for (int i = 0; i < 2 && audioOutput->bytesFree() > 192000; ++i) {
                Sample sample = demuxer->getSample(true);
                audioInput->write(reinterpret_cast<const char *>(sample.data), sample.len);
                demuxer->popSample();
            }
            if (nextFrame.isValid()) {
                syncTo(nextFrame.pts);
            }
        }
        audioOutput->suspend();
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
    bool isPlay = false;
public:
    HurricaneState state;

    HurricanePlayer(QQuickItem *parent = nullptr);

    virtual ~HurricanePlayer();

signals:

    /**
     * 状态发生改变
     */
    void stateChanged();
    void videoInit();

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
