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
    INVALID,       ///< 文件无效
    PRE_PLAY,      ///< 准备播放
    PLAYING,       ///< 正在播放
    STOPPED,       ///< 已停止
    LOADING,       ///< 正在加载
    PRE_PAUSE,     ///< 请求暂停
    PAUSED,        ///< 已暂停
};

class VideoPlayWorker : public QObject {
    Q_OBJECT
private:
    Demuxer *demuxer = nullptr;
    bool pauseRequested = false;
    QAudioSink *audioOutput;
    QIODevice *audioInput;

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

/**
 * 播放器 QuickItem 组件，提供暴露给 QML 的接口。
 * 我们完全采用异步通信的思想，这里的大部分方法都会直接返回。我们只需要关注一些事件（信号）更新 UI 的状态即可。
 * 我们的部分方法仅在特定状态才能使用，需要特别注意。例如：打开视频文件可以分为两个操作：stop（如果当前正在播放视频，需要先停止播放）和 openFile。
 * 也就是说，如果正在播放视频，必须先调用 stop 确保停止播放后再调用 openFile，否则 openFile 请求会被忽略。因此，一种正确的做法是先判断 state
 * 是否为 PLAYING（正在播放视频），如果不是，先调用 stop。由于我们的方法调用是异步的，所以其实我们不能保证方法返回后状态会马上发生改变。保险的做法
 * 需要关注 stateChanged 事件再进行下一步动作，这会大大增加编码的复杂度。幸运地，部分方法我们可以提供状态立刻发生变换的保证（见方法注释）。
 * 我们会在注释中说明调用的条件和状态转换。
 *
 * @see HurricaneState
 * @see HurricanePlayer::stateChanged()
 */
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
     * 打开视频文件, 需要保证 state 是 IDLED
     * @param path
     */
    Q_INVOKABLE void openFile(const QString &path);


    /**
     * 开始播放视频,
     */
    Q_INVOKABLE void start();

    /**
     * 停止播放视频, 能在 PLAYING 和 PAUSE 状态使用, 不能保证立即生效, 调用后状态立即转为 REQUEST_STOP, 生效后状态转为 STOP
     */
    Q_INVOKABLE void stop();

    /**
     * 暂停播放视频, 仅能在 PLAYING 状态使用, 不能保证立即生效, 调用后状态立即转为 REQUEST_PAUSE, 生效后状态转为 PAUSED
     * @see HurricanePlayer::stateChanged()
     */
    Q_INVOKABLE void pause();

    /**
     * 设置音量大小, 不能保证立即生效
     * @param v 音量大小, 通常在[0, 1]
     * @see HurricanePlayer::volumeChanged()
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

    /**
     * 改变视频播放的进度
     * @param pos 播放进度(单位: 秒)
     */
    Q_INVOKABLE void seek(qreal pos) { qDebug() << "HurricanePlayer: Seek" << pos; };
//
//    /**
//     * 将视频播放进度移动到指定位置
//     * 注：这个操作不会改变视频的播放状态
//     */


    void slotStateChanged(HurricaneState s) { state = s; emit stateChanged(); }

};


#endif //PONYPLAYER_QUICKITEM_H
