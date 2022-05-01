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
#include "demuxer2.h"

typedef int64_t time_point;
typedef int64_t time_duration;



namespace H {

    Q_NAMESPACE

    enum HurricaneState {
        CLOSING,       ///< 正在关闭当前文件, 这是一个瞬时状态
        LOADING,       ///< 正在加载, 这是一个瞬时状态
        INVALID,       ///< 文件无效
        PRE_PLAY,      ///< 准备播放, 这是一个瞬时状态, 将会很快转为 @code HurricaneState::PLAYING
        PLAYING,       ///< 正在播放
        PRE_PAUSE,     ///< 请求暂停, 这是一个瞬时状态
        PAUSED         ///< 已暂停
    };

    Q_ENUM_NS(HurricaneState)

}
using namespace H;

class VideoPlayWorker : public QObject {
    Q_OBJECT
private:
    Demuxer2 *demuxer = nullptr;
    bool pauseRequested = false;
    QAudioSink *audioOutput = nullptr;
    QIODevice *audioInput = nullptr;

    time_point seekPoint = 0;
    time_point idlePoint = -1;
    time_duration idleDurationSum = 0;

public:
    VideoPlayWorker();
    ~VideoPlayWorker() override { delete demuxer; }
    qreal getAudioDuration() { return demuxer->audioDuration(); }
    qreal getVideoDuration() { return demuxer->videoDuration(); }
private:
    inline time_point getAudioPlayingUSecs();
    inline void closeAudio();
    inline void syncTo(double pts);
public slots:
    void onAudioStateChanged(QAudio::State state);
    void slotOpenFile(const QString &path);
    void slotOpenFileResult(bool ret);
    void slotThreadInit();
    void slotOnWork();
    void slotClose();
    void slotVolumeChanged(qreal v);
    void slotPause();;
    void slotSeek(qreal pos);
signals:
    void signalImageChanged(Picture pic);
    void signalStateChanged(HurricaneState state);
    void signalPositionChangedBySeek();
    void signalVolumeChangedFail(qreal d);
    void signalDecoderOpenFile(std::string fn);
    void signalDecoderSeek(time_point pos);
    void signalDecoderStart();

};

/**
 * @brief
 * 播放器 QuickItem 组件，提供暴露给 QML 的接口。
 * @details
 * 我们完全采用异步通信的思想，这里的大部分方法都会直接返回。我们只需要关注一些事件（信号）更新 UI 的状态即可。我们的部分方法仅在特定状态才能
 * 使用，需要特别注意。例如：打开视频文件可以分为两个操作：stop（如果当前正在播放视频，需要先停止播放）和 openFile。也就是说，如果正在播放视
 * 频，必须先调用 stop 确保停止播放后再调用 openFile，否则 openFile 请求会被忽略。因此，一种正确的做法是先判断 state 是否为 PLAYING
 * （正在播放视频），如果不是，先调用 stop。由于我们的方法调用是异步的，所以其实我们不能保证方法返回后状态会马上发生改变。保险的做法需要关注
 * stateChanged 事件再进行下一步动作，这会大大增加编码的复杂度。幸运地，部分方法我们可以提供状态立刻发生变换的保证（见方法注释）。我们会在注
 * 释中说明调用的条件和状态转换。
 *
 * @see HurricaneState
 * @see HurricanePlayer::stateChanged()
 */
class HurricanePlayer : public Hurricane {
    Q_OBJECT
    Q_ENUM(HurricaneState)
    QML_ELEMENT
    Q_PROPERTY(HurricaneState state READ getState NOTIFY stateChanged FINAL)
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

signals:

    /**
     * 播放器状态发生改变.
     * 我们不能保证瞬时状态会转移到相应的稳定状态，所以我们不应该检测瞬时状态决定下一步的动作.
     * 例如: PRE_PAUSE 可以被 close 被打断, 直接转移到 INVALID 而永远不会转移到 PAUSED.
     */
    void stateChanged();

    /**
     * 播放器音量发生改变
     */
    void volumeChanged();

    /**
     * 打开文件结果
     * @param b 是否成功
     */
    void openFileResult(bool b);

    /**
     * 视频播放进度由于手动调整发送改变
     */
    void positionChangedBySeek();

    /**
     * 音频修改失败
     * @param current 当前的音量
     */
    void volumeChangedFail(qreal current);

Q_SIGNALS:

    // 下面这些方法用于与 VideoPlayWorker 通信
    // 约定两者通信的方法信号以 signal 开头, 槽函数以 slot 开头
    // 约定信号只能由所属的类的实例 emit
    void signalPlayerInitializing(QPrivateSignal);
    void signalPause(QPrivateSignal);
    void signalResume(QPrivateSignal);
    void signalClose(QPrivateSignal);
    void signalVolumeChanging(qreal v, QPrivateSignal);
    void signalOpenFile(const QString &url, QPrivateSignal);
    void signalSeek(qreal pos, QPrivateSignal);

public slots:

    /**
     * 打开视频文件
     * 需要保证调用时状态为 INVALID / CLOSING, 方法保证返回时状态为 LOADING
     * 如果指定 autoClose, 则会自动调用 HurricanePlayer::close()
     * 状态转移 INVALID -> LOADING -> PAUSED
     * @param path 文件路径
     * @param autoClose 如果打开视频文件, 是否自动关闭
     */
    Q_INVOKABLE void openFile(const QString &path, bool autoClose = true);


    /**
     * 开始播放视频
     * 需要保证调用时状态为 PAUSED / PRE_PAUSE, 方法保证返回时状态为 PRE_PLAY
     * 状态转移 PAUSED -> PRE_PLAY -> PLAYING
     */
    Q_INVOKABLE void start();

    /**
     * 暂停播放视频
     * 需要保证调用时状态为 PLAYING / PRE_PLAY, 方法保证返回时状态为 PRE_PAUSE
     * 状态转移 PAUSED -> PRE_PAUSE -> PAUSED
     */
    Q_INVOKABLE void pause();

    /**
     * 关闭当前播放的视频, 这会清空组件显示
     * 需要保证调用时的状态为 PAUSED / PRE_PAUSE, 方法保证返回时的状态为 CLOSING
     * 状态转移 PAUSED -> CLOSING -> INVALID
     */
    Q_INVOKABLE void close();

    /**
     * 设置音量大小, 不能保证立即生效, 请关注音量发送变化的信号
     * @param v 音量大小, 通常在[0, 1]
     * @see HurricanePlayer::volumeChanged()
     */
    Q_INVOKABLE void setVolume(qreal v);

    /**
     * 获取视频长度, 需要保证状态不是 INVALID
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getAudioDuration() { return videoPlayWorker.getAudioDuration(); }

    /**
     * 获取音频长度, 需要保证状态不是 INVALID
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getVideoDuration() { return videoPlayWorker.getVideoDuration(); }

    /**
     * 获取当前视频播放进度, 需要保证状态不是 INVALID
     * @return 播放进度(单位: 秒)
     */
    Q_INVOKABLE qreal getPTS() { return picture.getPTS(); }

    /**
     * 改变视频播放的进度, 不保证马上生效, 请关注信号
     * 需要保证当前状态为 PAUSE, PRE_PAUSE, PLAYING 或 PRE_PLAY
     * @param pos 播放进度(单位: 秒)
     * @see HurricanePlayer::positionChangedBySeek
     */
    Q_INVOKABLE void seek(qreal pos);;

private slots:
    void slotStateChanged(HurricaneState s);
    void slotPositionChangedBySeek() { emit positionChangedBySeek(); }
    void slotVolumeChangedFail(qreal current) { emit volumeChangedFail(current); }
};


#endif //PONYPLAYER_QUICKITEM_H
