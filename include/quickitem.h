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

class VideoPlayWorker : public QObject {
    Q_OBJECT
private:
    Demuxer *demuxer;
    Hurricane *hurricane;
    QAtomicInteger<bool> pauseRequested = false;
public:
    VideoPlayWorker(Demuxer *d, Hurricane *h) : QObject(nullptr), demuxer(d), hurricane(h) {

    }
    void resume() { pauseRequested = false; }
    void pause() { pauseRequested = true; }
public slots:
    void onWork() {
        qDebug() << "Start Video Work";
        Picture currFrame;
        while(!pauseRequested && (currFrame = demuxer->getPicture(true), currFrame.isValid())) {
            demuxer->popPicture();
            emit setImage(currFrame);
            Picture nextFrame = demuxer->getPicture(true);
            if (nextFrame.isValid()) {
                QThread::msleep(static_cast<unsigned long>((nextFrame.pts - currFrame.pts)  * 1000));
            }
        }
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
