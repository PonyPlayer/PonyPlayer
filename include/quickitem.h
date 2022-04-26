//
// Created by ColorsWind on 2022/4/21.
//
#pragma once
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


typedef int64_t time_point;
typedef int64_t time_duration;


enum class HurricaneState {
    IDLED, // 空闲状态, 没有打开文件
    INVALID, // 文件无效
    PLAYING, // 正在播放
    STOPPED, // 已停止
    LOADING, // 正在加载
    PAUSED,  // 已暂停
};

class VideoPlayWorker : public QObject {
    Q_OBJECT
private:
    Demuxer *demuxer = nullptr;
    bool pauseRequested = false;
    QAudioSink *audioOutput;
    QIODevice *audioInput;

//    QAtomicInteger<time_point>
    time_point seekPoint = 0;
    time_point idlePoint = -1;
    time_duration idleDurationSum = 0;

public:
    VideoPlayWorker() : QObject(nullptr) { demuxer = new Demuxer; }
    ~VideoPlayWorker() override { demuxer->quit(); delete demuxer; }
    qreal getAudioDuration() { return demuxer->audioDuration(); }
    qreal getVideoDuration() { return demuxer->videoDuration(); }
private:
    inline time_point getProcessedAudioUSecs();
    inline void syncTo(double pts);
public slots:
    void slotOpenFile(const QString &path);
    void onAudioStateChanged(QAudio::State state);
    void slotThreadInit();
    void slotOnWork();
    void slotVolumeChanged(qreal v) { audioOutput->setVolume(v); }
    void slotPause() {  pauseRequested = true; };
signals:
    void signalImageChanged(Picture pic);
    void signalStateChanged(HurricaneState state);
};


class HurricanePlayer : public Hurricane {
    Q_OBJECT
    QML_ELEMENT
    Q_ENUM(HurricaneState)
    Q_PROPERTY(HurricaneState state READ getState NOTIFY stateChanged FINAL)
    Q_PROPERTY(qreal volume READ getVolume NOTIFY volumeChanged FINAL)
private:
    HurricaneState state = HurricaneState::INVALID;
    qreal volume;
private:
    QThread *videoThread;
    VideoPlayWorker videoPlayWorker;
public:
    HurricanePlayer(QQuickItem *parent = nullptr);

    virtual ~HurricanePlayer();

    HurricaneState getState() { return state; }

    qreal getVolume() { return volume; }

signals:

    /**
     * 播放器状态发生改变
     */
    void stateChanged();

    /**
     * 播放器音量发生改变
     */
    void volumeChanged();


    // 下面这些方法用于与 VideoPlayWorker 通信
    // 约定两者通信的方法信号以 signal 开头, 槽函数以 slot 开头
    // 约定信号只能由所属的类的实例 emit
    void signalPlayerInitializing();
    void signalPause();
    void signalResume();
    void signalVolumeChanging(qreal v);
    void signalOpenFile(const QString &url);

public slots:

    /**
     * 打开视频文件, 需要保证 #state 是 IDLED
     * @param path
     */
    Q_INVOKABLE void openFile(const QString &path);

    /**
     * 关闭视频文件
     */
    Q_INVOKABLE void closeFile();

    /**
     * 开始播放视频
     */
    Q_INVOKABLE void start();

    /**
     * 停止播放视频
     */
    Q_INVOKABLE void stop();

    /**
     * 暂停播放视频
     */
    Q_INVOKABLE void pause();

    /**
     * 设置音量大小, 不能保证立即生效
     * @param v 音量大小, 通常在[0, 1]
     */
    Q_INVOKABLE void setVolume(qreal v) { emit signalVolumeChanging(v); }

    /**
     * 获取视频长度
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getAudioDuration() { return videoPlayWorker.getAudioDuration(); }

    /**
     * 获取音频长度
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getVideoDuration() { return videoPlayWorker.getVideoDuration(); }

    /**
     * 获取当前视频播放进度
     * @return 播放进度(单位: 秒)
     */
    Q_INVOKABLE qreal getPTS() { return picture.getPTS(); }
//
//    /**
//     * 将视频播放进度移动到指定位置
//     * 注：这个操作不会改变视频的播放状态
//     */


    void slotStateChanged(HurricaneState s) { state = s; emit stateChanged(); }

};


#endif //PONYPLAYER_QUICKITEM_H
