//
// Created by ColorsWind on 2022/5/6.
//

#pragma once
#ifndef PONYPLAYER_HURRICANE_HPP
#define PONYPLAYER_HURRICANE_HPP

#include <QObject>
#include <utility>
#include "framecontroller.hpp"
#include "fireworks.hpp"

/**
 * @brief
 * 播放器 QuickItem 组件，提供暴露给 QML 的接口。
 *
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
class Hurricane : public Fireworks {
Q_OBJECT
    QML_ADDED_IN_VERSION(1, 0)
    QML_ELEMENT
public:
    /**
     * HurricanePlayer 组件的状态, 其中瞬时状态代表前端 HurricanePlayer 发起操作, 但并未同步到后端 VideoWorker.
     * 由后端引起的状态改变不需要进入相应的瞬时状态. 如视频播放结束导致暂停的状态转换为: PLAYING -> PAUSED (中间不经过 PRE_PAUSED).
     */
    enum HurricaneState {
        CLOSING,       ///< 正在关闭当前文件, 这是一个瞬时状态
        LOADING,       ///< 正在加载, 这是一个瞬时状态
        INVALID,       ///< 文件无效
        PRE_PLAY,      ///< 准备播放, 这是一个瞬时状态, 将会很快转为 @code HurricaneState::PLAYING
        PLAYING,       ///< 正在播放
        PRE_PAUSE,     ///< 请求暂停, 这是一个瞬时状态
        PAUSED         ///< 已暂停
    };

    Q_ENUM(HurricaneState)

    QML_ELEMENT
    Q_PROPERTY(qreal pitch READ getPitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(HurricaneState state READ getState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QStringList audioDeviceList READ getAudioDeviceList NOTIFY audioOutputDeviceChanged)
    Q_PROPERTY(QStringList tracks READ getTracks NOTIFY openFileResult)
    Q_PROPERTY(bool backwardStatus READ isBackward NOTIFY backwardStatusChanged)
    Q_PROPERTY(int track READ getTrack WRITE setTrack NOTIFY trackChanged)
    Q_PROPERTY(
            QString currentOutputDevice READ getCurrentOutputDevice WRITE setCurrentOutputDevice NOTIFY currentOutputDeviceChanged)
    Q_PROPERTY(double speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)

private:
    bool backwardStatus = false;
private:
    HurricaneState state = HurricaneState::INVALID;
private:
    FrameController *frameController;
    int track = -1;
    double speed = 1.0;
public:
    explicit Hurricane(QQuickItem *parent = nullptr) : Fireworks(parent) {
        frameController = new FrameController(this);

        connect(this, &Hurricane::signalStart, frameController, &FrameController::start);
        connect(this, &Hurricane::signalPause, frameController, &FrameController::pause);

        connect(this, &Hurricane::signalOpenFile, frameController, &FrameController::openFile);

        connect(frameController, &FrameController::openFileResult, this, &Hurricane::slotOpenFileResult);
        connect(this, &Hurricane::signalClose, frameController, &FrameController::close);
        connect(frameController, &FrameController::setPicture, this, &Hurricane::setVideoFrame);

        connect(this, &Hurricane::signalSeek, frameController, &FrameController::seek);
        connect(frameController, &FrameController::signalPositionChangedBySeek, this,
                &Hurricane::slotPositionChangedBySeek);

        connect(frameController, &FrameController::playbackStateChanged, this, &Hurricane::slotPlaybackStateChanged);

        connect(this, &Hurricane::stateChanged, [this] {
            qDebug() << "State Changed to" << QVariant::fromValue(state).toString();
        });

        connect(frameController, &FrameController::signalAudioOutputDevicesChanged, this,
                &Hurricane::audioOutputDeviceChanged);
        connect(frameController, &FrameController::signalDeviceSwitched, this, &Hurricane::currentOutputDeviceChanged);
        emit signalPlayerInitializing(QPrivateSignal());
#ifdef DEBUG_FLAG_AUTO_OPEN
        openFile(QUrl::fromLocalFile(QDir::homePath().append(u"/581518754-1-208.mp4"_qs)).url());
#endif
    }

    virtual ~Hurricane() override {
        frameController->pause();
        frameController->deleteLater();
        qWarning() << "Destroy HurricanePlayer.";
    }

    HurricaneState getState() { return state; }

    int getTrack() {
        return track;
    }

    double getSpeed() {
        return speed;
    }

    bool isBackward() { return backwardStatus; }

    QStringList getAudioDeviceList() { return frameController->getAudioDeviceList(); }


signals:

    /**
     * 播放器状态发生改变.
     * 我们不能保证瞬时状态会转移到相应的稳定状态，所以我们不应该检测瞬时状态决定下一步的动作.
     * 例如: PRE_PAUSE 可以被 close 被打断, 直接转移到 INVALID 而永远不会转移到 PAUSED.
     */
    void stateChanged();

    /**
     * 打开文件结果
     * @param b 是否成功
     */
    void openFileResult(PonyPlayer::OpenFileResultType result, QPrivateSignal);

    /**
     * 视频播放进度由于手动调整发送改变
     */
    void positionChangedBySeek();

    void audioOutputDeviceChanged();

    void backwardStatusChanged();

    void trackChanged();

    void currentOutputDeviceChanged();

    void pitchChanged();

    void speedChanged();


Q_SIGNALS:

    // 下面这些方法用于与 VideoPlayWorker 通信
    // 约定两者通信的方法信号以 signal 开头, 槽函数以 slot 开头
    // 约定信号只能由所属的类的实例 emit
    void signalPlayerInitializing(QPrivateSignal);

    void signalStart(QPrivateSignal);

    void signalPause(QPrivateSignal);

    void signalClose(QPrivateSignal);

    void signalOpenFile(const QString &path, QPrivateSignal);

    void signalSeek(qreal pos, QPrivateSignal);


public slots:

    /**
     * 打开视频文件
     * 需要保证调用时状态为 INVALID / CLOSING, 方法保证返回时状态为 LOADING
     * 如果指定 autoClose, 则会自动调用 HurricanePlayer::close()
     * 状态转移 INVALID -> LOADING -> PAUSED
     * @param url 文件路径
     * @param autoClose 如果打开视频文件, 是否自动关闭
     */
    Q_INVOKABLE void openFile(const QString &url, bool autoClose = true) {
        if (autoClose && (state == HurricaneState::PLAYING || state == PRE_PLAY)) {
            emit pause();
        }
        if (autoClose && (state == HurricaneState::PAUSED || state == PRE_PAUSE)) {
            emit close();
        }
        if (state == HurricaneState::CLOSING || state == HurricaneState::INVALID) {
            if (state == HurricaneState::CLOSING) {
                // wait for the operation to complete
                QCoreApplication::processEvents();
            }
            state = HurricaneState::LOADING;
            emit stateChanged();
            backwardStatus = false;
            emit backwardStatusChanged();
            emit signalOpenFile(QUrl(url).toLocalFile(), QPrivateSignal());
            qDebug() << "Open file:" << url;
        }
    };


    /**
     * 开始播放视频
     * 需要保证调用时状态为 PAUSED / PRE_PAUSE, 方法保证返回时状态为 PRE_PLAY
     * 状态转移 PAUSED -> PRE_PLAY -> PLAYING
     */
    Q_INVOKABLE void start() {
        if (state == HurricaneState::PAUSED || state == HurricaneState::PRE_PAUSE) {
            state = HurricaneState::PRE_PLAY;
            emit stateChanged();
            emit signalStart(QPrivateSignal());
            qDebug() << "Start play video.";
        }
    }

    /**
     * 暂停播放视频, 保证后端尽快停止视频播放进入空闲状态.
     * 需要保证调用时状态为 PLAYING / PRE_PLAY, 方法保证返回时状态为 PRE_PAUSE
     * 状态转移 PAUSED -> PRE_PAUSE -> PAUSED
     */
    Q_INVOKABLE void pause() {
        if (state == HurricaneState::PLAYING || state == HurricaneState::PRE_PLAY) {
            state = HurricaneState::PRE_PAUSE;
            emit stateChanged();
            emit signalPause(QPrivateSignal());
            qDebug() << "Pause.";
        }
    }

    /**
     * 关闭当前播放的视频, 这会清空组件显示
     * 需要保证调用时的状态为 PAUSED / PRE_PAUSE, 方法保证返回时的状态为 CLOSING
     * 状态转移 PAUSED -> CLOSING -> INVALID
     */
    Q_INVOKABLE void close() {
        if (state == HurricaneState::PRE_PAUSE || state == HurricaneState::PAUSED) {
            state = HurricaneState::CLOSING;
            emit stateChanged();
            this->setVideoFrame(VideoFrameRef());
            emit signalClose(QPrivateSignal());
        }
    }

    /**
     * 设置音量大小, 音频的范围为[0, 1]
     * @param volume 音量大小
     */
    Q_INVOKABLE void setVolume(qreal volume) {
        frameController->setVolume(volume);
    }

    /**
     * 设置音调
     * @param pitch 音调
     */
    Q_INVOKABLE void setPitch(qreal pitch) {
        frameController->setPitch(pitch);
    }

    /**
     * 设置速度倍率, 要求倍率 > 0
     * @param speed 速度倍率
     */
    Q_INVOKABLE void setSpeed(qreal speed) {
        frameController->setSpeed(speed);
        this->speed = speed;
        emit speedChanged();
    }

    /**
     * 设置音频输出设备名称
     * @param deviceName 设备名称
     */
    Q_INVOKABLE void setCurrentOutputDevice(QString deviceName) {
        frameController->setSelectedAudioOutputDevice(std::move(deviceName));
    }

    /**
     * 获取音频输出设备列表
     */
    Q_INVOKABLE QString getCurrentOutputDevice() {
        return frameController->getSelectedAudioOutputDevice();
    }

    /**
     * 获取视频长度, 需要保证状态不是 INVALID
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getAudioDuration() { return frameController->getAudioDuration(); }

    /**
     * 获取音频长度, 需要保证状态不是 INVALID
     * @return 长度(单位: 秒)
     */
    Q_INVOKABLE qreal getVideoDuration() { return frameController->getVideoDuration(); }

    /**
     * 获取当前视频播放进度, 需要保证状态不是 INVALID
     * @return 播放进度(单位: 秒)
     */
    Q_INVOKABLE qreal getPTS() {
        qreal pts = frameController->getPreferablePos();
        if (std::isnan(pts)) {
            qWarning() << "getPTS return NaN";
        }
        return pts;
    }

    /**
     * 改变视频播放的进度, 不保证马上生效, 请关注信号
     * 需要保证当前状态为 PAUSE, PRE_PAUSE, PLAYING 或 PRE_PLAY
     * @param pos 播放进度(单位: 秒)
     * @see HurricanePlayer::positionChangedBySeek
     */
    Q_INVOKABLE void seek(qreal pos) {
        // only available on PLAY/PAUSE
        bool playing = false;
        switch (state) {
            case HurricaneState::PLAYING:
            case HurricaneState::PRE_PLAY:
                playing = true;
                /* fall through */
            case HurricaneState::PAUSED:
            case HurricaneState::PRE_PAUSE:
                break;
            default:
                return;
        }
        if (pos < 0 || pos > getVideoDuration())
            return;
        state = PRE_PAUSE;
        emit stateChanged();
//    emit signalPause(QPrivateSignal());
        emit signalSeek(pos, QPrivateSignal());
        if (playing) {
            emit signalStart(QPrivateSignal());
        }
        qDebug() << "HurricanePlayer: Seek" << pos;
    }

    Q_INVOKABLE QStringList getTracks() {
        if (state == LOADING || state == INVALID) {
            qWarning() << "Get tracks when" << state;
        }
        return frameController->getTracks();
    }

    Q_INVOKABLE void setTrack(int i) {
        bool playing = false;
        switch (state) {
            case HurricaneState::PLAYING:
            case HurricaneState::PRE_PLAY:
                playing = true;
                /* fall through */
            case HurricaneState::PAUSED:
            case HurricaneState::PRE_PAUSE:
                break;
            default:
                return;
        }
        state = PRE_PAUSE;
        emit stateChanged();
        frameController->setTrack(i);
        track = i;
        emit trackChanged();
        if (playing) {
            emit signalStart(QPrivateSignal());
        }

        qDebug() << "Set Music Track" << i;
    }


    Q_INVOKABLE void forward() {
        bool playing = false;
        switch (state) {
            case HurricaneState::PLAYING:
            case HurricaneState::PRE_PLAY:
                playing = true;
                /* fall through */
            case HurricaneState::PAUSED:
            case HurricaneState::PRE_PAUSE:
                break;
            default:
                return;
        }
        state = PRE_PAUSE;
        emit stateChanged();
        backwardStatus = false;
        emit backwardStatusChanged();
        frameController->forward();
        if (playing) {
            emit signalStart(QPrivateSignal());
        }
        qDebug() << "forward";
    }

    Q_INVOKABLE void backward() {
        bool playing = false;
        switch (state) {
            case HurricaneState::PLAYING:
            case HurricaneState::PRE_PLAY:
                playing = true;
                /* fall through */
            case HurricaneState::PAUSED:
            case HurricaneState::PRE_PAUSE:
                break;
            default:
                return;
        }
        state = PRE_PAUSE;
        emit stateChanged();
        backwardStatus = true;
        emit backwardStatusChanged();
        frameController->backward();
        if (playing) {
            emit signalStart(QPrivateSignal());
        }
        qDebug() << "backward";
    }

    Q_INVOKABLE void toggleBackward() {
        if (isBackward()) {
            forward();
        } else {
            backward();
        }
    }

    Q_INVOKABLE bool hasVideo() {
        return frameController != nullptr && frameController->hasVideo();
    }

    Q_INVOKABLE qreal getPitch() {
        return frameController ? frameController->getPitch() : 1.0;
    }


private slots:

    void slotPositionChangedBySeek() { emit positionChangedBySeek(); }

    void slotPlaybackStateChanged(bool isPlaying) {
        if (isPlaying) {
            state = PLAYING;
        } else {
            state = PAUSED;
        }
        emit stateChanged();
    };

    void slotOpenFileResult(PonyPlayer::OpenFileResultType result) {
        if (result != PonyPlayer::OpenFileResultType::FAILED) {
            state = PAUSED;
            track = 0;
        } else {
            state = INVALID;
            track = -1;
        }
        emit openFileResult(result, QPrivateSignal());
        emit trackChanged();
        emit stateChanged();
    }


};


#endif //PONYPLAYER_HURRICANE_HPP
