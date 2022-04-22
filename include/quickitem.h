//
// Created by ColorsWind on 2022/4/21.
//

#ifndef PONYPLAYER_QUICKITEM_H
#define PONYPLAYER_QUICKITEM_H

#include <QRunnable>
#include <QAtomicInt>
#include <QWaitCondition>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <chrono>
#include "hurricane.h"
#include "demuxer.h"

class HurricanePlayer;

using std::chrono::system_clock;

class HurricaneState : public QObject {
Q_OBJECT
    QML_ELEMENT
public:

};

/**
 * 负责按时取出视频帧并设置 Image
 */
class VideoTimer : public QThread {
    Q_OBJECT
private:
    QMutex mutex;
    QWaitCondition condition;
public:
    struct NextFrameEvent {
        QAtomicInt block = true;
        QAtomicInteger<uint64_t> next;
    } frameEvent;

    VideoTimer(QObject *parent = nullptr);
    void run() override;
    void notify() {
        condition.wakeAll();
    }

signals:
    void timeEmit(); //NOLINT
};


class HurricanePlayer : public Hurricane {
    Q_OBJECT
    QML_ELEMENT
//    Q_PROPERTY(HurricaneState state NOTIFY stateChanged)
private:
    Demuxer demuxer;
    VideoTimer timer;
    QThread timerThread;
    clock_t videoStartTime;
    Picture picture;
public:
    HurricaneState state;

    HurricanePlayer(QQuickItem *parent = nullptr);

signals:

    /**
     * 状态发生改变
     */
    void stateChanged();

    /**
     * 视频播放完
     */
    void playFinished();

public slots:

    void playNextFrame();
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
