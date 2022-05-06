//
// Created by ColorsWind on 2022/5/5.
//
#pragma once
#ifndef PONYPLAYER_VIDEOWORKER_H
#define PONYPLAYER_VIDEOWORKER_H
#include <QObject>
#include <QThread>
#include "frame_queue.h"
#include "demuxer.hpp"
#include "audiosink.h"

/**
 * @brief 负责输出视频和音频.
 *
 * 这个类负责将上层的帧输出到相应的设备. 这个类的RAII的. 如果没有特殊说明, 这个类的公有方法是线程安全的.
 */
class Playback : public QObject {
    Q_OBJECT
private:
    QThread *m_affinityThread;
    Demuxer *m_demuxer;


    PonyAudioSink *m_audioSink;
    std::atomic<bool> m_isInterrupt;
    std::atomic<bool> m_isPlaying;
    std::mutex m_interruptMutex;
    std::mutex m_workMutex;
    std::condition_variable m_interruptCond;

    inline void changeState(bool isPlaying) {
        m_isPlaying = isPlaying;
        emit stateChanged(isPlaying);
    }

    inline void syncTo(double pts) {
        double duration = pts - m_audioSink->getProcessSecs();
        if (duration > 0) {
            if (duration > 1) { qWarning() << "Sleep long duration" << duration << "s"; }
            std::unique_lock lock(m_interruptMutex);
            m_interruptCond.wait_for(lock, std::chrono::duration<double>(duration));
        } else {
            qWarning() << "Sleep negative duration" << duration << "s";
        }
    }

    inline bool writeAudio(int batch) {
        for (int i = 0; i < batch && m_audioSink->freeByte() > MAX_AUDIO_FRAME_SIZE; ++i) {
            Sample sample = m_demuxer->getSample(true);
            if (!sample.isValid()) { return false; }
            m_demuxer->popSample(true);
            m_audioSink->write(reinterpret_cast<const char *>(sample.data), sample.len);
            sample.free();
        }
        return true;
    }

public:
    Playback(Demuxer *demuxer, QObject *parent): QObject(nullptr), m_demuxer(demuxer) {
        m_affinityThread = new QThread{parent};
        m_affinityThread->setObjectName("PlayThread");
        this->moveToThread(m_affinityThread);
        connect(this, &Playback::startWork, this, &Playback::onWork);
        connect(m_affinityThread, &QThread::started, [=]{
            QAudioFormat format;
            format.setSampleRate(44100);
            format.setChannelCount(2);
            format.setSampleFormat(QAudioFormat::Int16);
            this->m_audioSink = new PonyAudioSink(format, MAX_AUDIO_FRAME_SIZE * 2);
        });
        m_affinityThread->start();
    }

    virtual ~Playback() {
//        m_affinityThread->quit();
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

    /**
     * 开始进行处理, 发送信号后方法将立即返回.
     */
    void start(qreal startPoint=0.0) {
        m_isInterrupt = false;
        m_audioSink->setStartPoint(startPoint);
        emit startWork(QPrivateSignal());
    }

    /**
     * 尽快暂停处理, 这个方法将会阻塞直到当前工作停止. 这个方法不会丢失数据.
     */
    void pause() {
        m_isInterrupt = true;
        m_interruptCond.notify_all();
        std::unique_lock lock(m_workMutex);
    }

    /**
     * 立即停止, 这个方法将会阻塞直到当前工作停止. 这个方法将会放弃缓冲区中的所有数据.
     */
    void stop() {
        pause();
        m_audioSink->stop();
    }

private slots:

    /**
     * 播放音视频. 需要保证 demuxer 可以正常阻塞.
     */
    void onWork() {
        std::unique_lock lock(m_workMutex);
        changeState(true);
        writeAudio(5);
        m_audioSink->start();
        while(!m_isInterrupt) {
            Picture pic = m_demuxer->getPicture(true);
            if (!pic.isValid()) { break; }
            emit setPicture(pic);
            m_demuxer->popPicture(true);
            if (!writeAudio(10)) { break; }
            Picture next = m_demuxer->getPicture(true);
            if (next.isValid()) { syncTo(next.getPTS()); }
        }
        m_audioSink->pause();
        changeState(false);
    };


signals:
    void startWork(QPrivateSignal);
    void setPicture(Picture pic);
    void stateChanged(bool isPlaying);
};

#endif //PONYPLAYER_VIDEOWORKER_H
