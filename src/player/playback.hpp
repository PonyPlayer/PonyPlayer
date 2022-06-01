//
// Created by ColorsWind on 2022/5/5.
//
#pragma once
#ifndef PONYPLAYER_VIDEOWORKER_H
#define PONYPLAYER_VIDEOWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <utility>
#include "demuxer.hpp"
#include "audiosink.hpp"
#include "frame.hpp"

/**
 * @brief 负责输出视频和音频(不含视频预览).
 *
 * 这个类负责将上层的帧输出到相应的设备. 这个类的RAII的. 如果没有特殊说明, 这个类的公有方法是线程安全的.
 */
class Playback : public QObject {
Q_OBJECT
private:
    QThread *m_affinityThread;
    Demuxer *m_demuxer;
    VideoFrameRef cacheVideoFrame;


    PonyAudioSink *m_audioSink = nullptr;
    std::atomic<bool> m_isInterrupt;
    std::atomic<bool> m_isPlaying;
    std::mutex m_interruptMutex;
    std::mutex m_workMutex;
    std::condition_variable m_interruptCond;

    // playback 的速度, 不受倍速限制
    qreal m_speedFactor = 1.0;

    std::atomic<qreal> m_preferablePos = 0.0;

    inline void changeState(bool isPlaying) {
        m_isPlaying = isPlaying;
        emit stateChanged(isPlaying);
    }

    inline void syncTo(qreal current) {
        bool backward = m_demuxer->isBackward();
        double duration;
        if (!m_demuxer->hasVideo()) {
            // 纯音频
            duration = 1. / 30;
            m_preferablePos = m_audioSink->getProcessSecs(backward);
        } else {
            qreal pos = m_demuxer->frontPicture();
            if (isnan(pos)) { return; }
            m_preferablePos = current;
            if (m_audioSink->isBlock()) {
                // 由于没有音频
                duration = (current - pos) / m_audioSink->speed();
            } else {
                if (m_audioSink->speed() > 2 - 1e-5) {
                    if (!backward) {
                        m_demuxer->skipPicture([this, backward](qreal framePos) {
                            return framePos < m_audioSink->getProcessSecs(backward);
                        });
                    } else {
                        m_demuxer->skipPicture([this, backward](qreal framePos) {
                            return framePos > m_audioSink->getProcessSecs(backward);
                        });
                    }
                }
                duration = m_demuxer->frontPicture() - m_audioSink->getProcessSecs(backward);
            }
            if (backward) { duration = -duration; }
        }
        if (duration > 0) {
            if (duration > 1) {
                qWarning() << "Sleep long duration" << duration << "s";
            }
            std::unique_lock lock(m_interruptMutex);
            if (!m_isInterrupt) {
                m_interruptCond.wait_for(lock, std::chrono::duration<double>(duration));
            }
        } else {
            qWarning() << "Sleep negative duration" << duration << "s";
        }
    }

    inline bool writeAudio(int batch) {
        if (m_audioSink->isBlock()) { return true; }
        for (int i = 0; i < batch && m_audioSink->freeByte() > MAX_AUDIO_FRAME_SIZE; ++i) {
            AudioFrame sample = m_demuxer->getSample();
            if (!sample.isValid()) { return false; }
            m_audioSink->write(reinterpret_cast<const char *>(sample.getSampleData()), sample.getDataLen());
        }
        return true;
    }

    PONY_GUARD_BY(PLAYBACK)

    VideoFrameRef getVideoFrame() {
        if (cacheVideoFrame.isValid()) {
            VideoFrameRef ret = std::move(cacheVideoFrame);
            cacheVideoFrame = {};
            return ret;
        } else {
            return m_demuxer->getPicture();
        }
    }

public:
    Playback(Demuxer *demuxer, QObject *parent) : QObject(nullptr), m_demuxer(demuxer) {
        m_affinityThread = new QThread;
        m_affinityThread->setObjectName(PonyPlayer::PLAYBACK);
        this->moveToThread(m_affinityThread);
        connect(this, &Playback::startWork, this, &Playback::onWork);
        connect(this, &Playback::stopWork, this, [this] { this->m_audioSink->stop(); });
        connect(this, &Playback::setAudioStartPoint, this, [this](qreal t) { this->m_audioSink->setStartPoint(t); });
        connect(this, &Playback::setAudioVolume, this, [this](qreal volume) { this->m_audioSink->setVolume(volume); });
        connect(this, &Playback::setAudioPitch, this, [this](qreal pitch) { this->m_audioSink->setPitch(pitch); });
        connect(this, &Playback::setAudioSpeed, this, [this](qreal speed) {
            m_speedFactor = speed;
            this->m_audioSink->setSpeed(speed);
            if (speed > PonyAudioSink::MAX_SPEED_FACTOR) {
                if (this->m_audioSink->isBlock()) { return; }
                // 需要禁用音频
                this->m_audioSink->setBlockState(true);
                emit requestResynchronization(false, false); // queue connection
            } else if (speed <= PonyAudioSink::MAX_SPEED_FACTOR) {
                if (!this->m_audioSink->isBlock()) { return; }
                // 需要重新启动音频
                this->m_audioSink->setBlockState(false);
                emit requestResynchronization(true, false); // queue connection
            }
        });
        connect(this, &Playback::showFirstVideoFrame, this, [this] {
            if (!cacheVideoFrame.isValid()) { cacheVideoFrame = m_demuxer->getPicture(); }
            emit setPicture(cacheVideoFrame);
        });
        connect(this, &Playback::clearRingBuffer, this, [this] { this->m_audioSink->clear(); });
        connect(m_affinityThread, &QThread::started, [this] {
            // 在 Playback 线程上初始化
            PonyAudioFormat format(PonyPlayer::Int16, 44100, 2); // default format
            this->m_audioSink = new PonyAudioSink(format);
            connect(m_audioSink, &PonyAudioSink::signalDeviceSwitched, this, [this] {
                emit signalDeviceSwitched();
                emit requestResynchronization(!this->m_audioSink->isBlock(), true);
            }, Qt::QueuedConnection);
            connect(this, &Playback::signalSetSelectedAudioOutputDevice, m_audioSink,
                    &PonyAudioSink::requestDeviceSwitch);
            connect(m_audioSink, &PonyAudioSink::signalAudioOutputDeviceListChanged, this, [this] {
                emit signalAudioOutputDevicesListChanged();
            });
            emit signalAudioOutputDevicesListChanged();
        });
        m_affinityThread->start();
    }

    PONY_THREAD_SAFE qreal getPreferablePos() {
        return m_preferablePos;
    }

    PonyAudioFormat getDeviceFormat() {
        std::unique_lock lock(m_workMutex);
        return m_audioSink->getCurrentDeviceFormat();
    }

    [[nodiscard]] qreal getAudioPos(bool backward) const {
        if (m_speedFactor < PonyAudioSink::MAX_SPEED_FACTOR) {
            return m_audioSink->getProcessSecs(backward);
        } else {
            ILLEGAL_STATE("AudioPos not available.");
        }
    }


    void setDesiredFormat(const PonyAudioFormat& format) {
        m_audioSink->setFormat(format);
    }

//    qreal getVideoPos() const {
//        return m_preferablePos.load();
//    }

    virtual ~Playback() {
        m_affinityThread->quit();
    }

    void setVolume(qreal volume) {
        emit setAudioVolume(volume, QPrivateSignal());
    }

    void setPitch(qreal pitch) {
        emit setAudioPitch(pitch, QPrivateSignal());
    }

    void setSpeed(qreal speed) {
        emit setAudioSpeed(speed, QPrivateSignal());
    }

    void setSelectedAudioOutputDevice(QString deviceName) {
        emit signalSetSelectedAudioOutputDevice(std::move(deviceName));
    }

    QString getSelectedAudioOutputDevice() {
        return m_audioSink ? m_audioSink->getSelectedOutputDevice() : "";
    }

    void showFrame() {
        emit showFirstVideoFrame(QPrivateSignal());
    }

    /**
     * 是否正在播放
     * @return 状态
     */
    bool isPlaying() { return m_isPlaying; }

    /**
     * 是否请求停止
     * @return 状态
     */
    bool isInterrupted() { return m_isInterrupt; }

    void setStartPoint(qreal startPoint) {
        m_isInterrupt = false;
//        m_preferablePos = startPoint;
        qDebug() << "SetStartPoint" << startPoint;
        emit setAudioStartPoint(startPoint, QPrivateSignal());
    }

    /**
     * 开始进行处理, 发送信号后方法将立即返回.
     */
    void start() {
//        std::unique_lock lock(m_workMutex);
        m_isInterrupt = false;
        emit startWork(QPrivateSignal());
    }

    /**
     * 清空内部缓冲区, 需要保证此刻没有读写操作.
     */
    void clear() {
        emit clearRingBuffer(QPrivateSignal());
    }

    /**
     * 尽快暂停处理, 这个方法将会阻塞直到当前工作停止. 这个方法不会丢失数据.
     */
    void pause() {
        std::unique_lock cond_lock(m_interruptMutex);
        m_isInterrupt = true;
        m_interruptCond.notify_all();
        cond_lock.unlock();
        std::unique_lock lock(m_workMutex);
    }

    /**
     * 立即停止, 清空缓冲区的数据.
     */
    void stop() {
        std::unique_lock cond_lock(m_interruptMutex);
        m_isInterrupt = true;
        m_interruptCond.notify_all();
        cond_lock.unlock();
        std::unique_lock lock(m_workMutex); // make sure stop
        emit stopWork(QPrivateSignal());
        emit setAudioStartPoint(0.0, QPrivateSignal());
        emit clearRingBuffer(QPrivateSignal());
    }

    QStringList getAudioDeviceList() { return m_audioSink ? m_audioSink->getAudioDeviceList() : QStringList(); };

    qreal getPitch() { return m_audioSink ? m_audioSink->pitch() : 1.0; }


private slots:

    /**
     * 播放音视频. 需要保证 demuxer 可以正常阻塞.
     */
    void onWork() {
        std::unique_lock lock(m_workMutex, std::defer_lock);
        if (!lock.try_lock()) { return; } // not allow neat run
        changeState(true);
        writeAudio(5);
        m_audioSink->start();
        while (!m_isInterrupt) {
            VideoFrameRef pic = getVideoFrame();
            if (!pic.isValid()) {
                m_audioSink->waitComplete();
                emit resourcesEnd();
                break;
            }
//            m_videoPos = pic.getPTS();
            emit setPicture(pic);
            if (!writeAudio(static_cast<int>(10 * m_audioSink->speed()))) {
                m_audioSink->waitComplete();
                emit resourcesEnd();
                break;
            }
            QCoreApplication::processEvents(); // process setVolume setSpeed etc
            syncTo(pic.getPTS());
        }
        m_audioSink->pause();
        changeState(false);
        lock.unlock();
    };



signals:

    void startWork(QPrivateSignal);

    void stopWork(QPrivateSignal);

    void clearRingBuffer(QPrivateSignal);

    void setAudioStartPoint(qreal startPoint, QPrivateSignal);

    void setAudioVolume(qreal volume, QPrivateSignal);

    void setAudioPitch(qreal pitch, QPrivateSignal);

    void setAudioSpeed(qreal speed, QPrivateSignal);

    void signalSetSelectedAudioOutputDevice(QString);

    void signalDeviceSwitched();

    void showFirstVideoFrame(QPrivateSignal);

    void setPicture(VideoFrameRef pic);

    void stateChanged(bool isPlaying);

    void resourcesEnd();

    void signalAudioOutputDevicesListChanged();

    /**
     * 由于设备切换, 音频倍速调整等原因需要下层重新同步
     */
    void requestResynchronization(bool enableAudio, bool updateAudioFormat);


};

#endif //PONYPLAYER_VIDEOWORKER_H
