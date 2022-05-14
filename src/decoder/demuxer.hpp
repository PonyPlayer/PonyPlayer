//
// Created by ColorsWind on 2022/5/6.
//
#pragma once

#include <QObject>
#include "dispatcher.hpp"

class Demuxer : public QObject {
    Q_OBJECT
    PONY_THREAD_AFFINITY(DECODER)

private:
    // openfile(MainThread) -> openfile(FrameThread) -> openfile(Decoder) -> openfile(MainThread)
    // no addition synchronization is needed
    DemuxDispatcherBase *m_worker = nullptr;
    DecodeDispatcher *m_forward;
    ReverseDecodeDispatcher *m_backward;

    QThread *m_affinityThread = nullptr;
    FrameFreeQueue m_freeQueue;
    FrameFreeFunc m_freeFunc;

    std::mutex mutex;
public:
    Demuxer(QObject *parent) : QObject(nullptr), m_freeQueue(1024) {
        m_affinityThread = new QThread;
        m_affinityThread->setObjectName("DecoderThread");
        this->moveToThread(m_affinityThread);
        m_affinityThread->start();
        m_freeFunc = [this](AVFrame *frame){m_freeQueue.enqueue(frame);};
    }

    ~Demuxer() {
        qDebug() << "Destroy Demuxer";
        m_affinityThread->quit();
    }

    PONY_GUARD_BY(MAIN, FRAME, DECODER) VideoFrame getPicture(bool b, bool own) { return m_worker->getPicture(b, own); }

    PONY_GUARD_BY(MAIN, FRAME, DECODER) bool popPicture(bool b) { return m_worker->popPicture(b); }

    PONY_GUARD_BY(MAIN, FRAME, DECODER) AudioFrame getSample(bool b) { return m_worker->getSample(b); }


    PONY_GUARD_BY(MAIN, FRAME, DECODER) bool popSample(bool b) { return m_worker->popSample(b); }

    PONY_CONDITION("OpenFileResult")
    PONY_GUARD_BY(MAIN, FRAME, DECODER) qreal audioDuration() { return m_forward ? m_forward->getAudionLength() : 0.0; }

    PONY_CONDITION("OpenFileResult")
    PONY_GUARD_BY(MAIN, FRAME, DECODER) qreal videoDuration() { return m_forward ? m_forward->getVideoLength() : 0.0; }

    PONY_CONDITION("OpenFileResult")
    PONY_GUARD_BY(MAIN, FRAME, DECODER) QStringList getTracks() {
        if (m_forward) {
            return m_forward->getTracks();
        } else {
            return {u"Not Open File"_qs};
        }
    }

    /**
     * 当前是否倒放
     * @return
     */
    PONY_GUARD_BY(MAIN, FRAME, DECODER) bool isRewind() {
        return dynamic_cast<ReverseDecodeDispatcher*>(m_worker);
    }

    /**
     * 向 DecodeThread 发送信号尽快暂停解码, 并唤醒阻塞在上面的线程.
     * @see DecodeDispatcher::statePause
    */
    PONY_THREAD_SAFE void pause() {
        m_worker->statePause();
    }

    /**
     * 清空旧的帧, 这个方法会阻塞直到队列中的所有旧帧清理完成.
     * @see DecodeDispatcher::flush
     */
    PONY_GUARD_BY(FRAME) void flush() {
        m_worker->flush();
    }

    /**
     * 在 DecodeThread 启动解码器, 这个方法是非阻塞的, 但是可以保证返回后队里请求能够被阻塞.
     */
    PONY_THREAD_SAFE void start() {
        qDebug() << "Start Decoder";
        m_worker->stateResume();
        emit signalStartWorker(QPrivateSignal());
    }

    PONY_GUARD_BY(FRAME) void setTrack(int i) {
        m_worker->setTrack(i);
    }

public slots:

    /**
     * 调整视频进度, 必须保证解码器线程空闲且缓冲区为空. 方法返回后保证产生的帧是在时间正确. \n
     * 一次完整的调整进度操作应该为: \n
     * 1. 在VideoThread线程调用 Demuxer2::statePause 使解码器线程停止运行; \n
     * 2. 在VideoThread线程调用 Demuxer2::seek 并阻塞等待函数返回, 接下来产生的帧是新的帧. \n
     * 3. 在VideoThread线程调用 Demuxer2::flush 清空队列中的旧帧. \n
     * 4. 在DeocdeThread线程中执行 Demuxer2::start, 恢复解码器线程线程运行. \n
     * @param secs 视频进度(单位: s)
     * @see Demuxer2::statePause
     * @see Demuxer2::flush
     * @see DecodeDispatcher::seek
     */
    void seek(qreal secs) {
        m_worker->seek(secs);
    }

    /**
     * 设置音频索引, 必须保证解码器线程空闲且缓冲区为空
     * @param index
     * @see DecodeDispatcher::seek
     */
    void setAudioIndex(StreamIndex index) {
        m_worker->setAudioIndex(index);
    }

    /**
     * 打开文件
     * @param fn 本地文件路径
     */
    void openFile(const std::string &fn) {
        qDebug() << "Open file" << QString::fromUtf8(fn);
        // call on video decoder thread
        if (m_worker) {
            qWarning() << "Already open file:" << m_worker->filename.c_str();
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        std::unique_lock lock(mutex);
        try {
            m_forward = new DecodeDispatcher(fn, &m_freeQueue, &m_freeFunc, DEFAULT_STREAM_INDEX, DEFAULT_STREAM_INDEX, this);
            m_backward = new ReverseDecodeDispatcher(fn, this);
            m_worker = m_forward;
        } catch (std::runtime_error &ex) {
            qWarning() << "Error opening file:" << ex.what();
            m_worker = nullptr;
            m_backward = nullptr;
            m_forward = nullptr;
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        lock.unlock();
        connect(this, &Demuxer::signalStartWorker, m_worker, &DemuxDispatcherBase::onWork);
        emit openFileResult(true, QPrivateSignal());
    }



    /**
     * 倒放视频, 必须保证解码器线程空闲且缓冲区为空. 方法返回后保证产生的帧是在时间正确.
     * @param secs 视频进度(单位: s)
     * @see Demuxer2::statePause
     * @see Demuxer2::flush
     * @see DecodeDispatcher::seek
     */
    void reverse() {

    }

    /**
     * 正向播放视频, 必须保证解码器线程空闲且缓冲区为空. 方法返回后保证产生的帧是在时间正确.
     * @param secs 视频进度(单位: s)
     * @see Demuxer2::statePause
     * @see Demuxer2::flush
     * @see DecodeDispatcher::seek
     */
    void forward() {

    };

    void close() {
        if (m_worker) {
            qDebug() << "Close file" << m_worker->filename.c_str();
            m_worker->statePause();
            m_worker->deleteLater();
            m_worker = nullptr;
        } else {
            qWarning() << "Try to close file while no file has been opened.";
        }
    }

signals:

    void signalStartWorker(QPrivateSignal);

    void openFileResult(bool ret, QPrivateSignal);



};

