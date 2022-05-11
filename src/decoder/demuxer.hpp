//
// Created by ColorsWind on 2022/5/6.
//
#pragma once

#include <QObject>
#include "dispatcher.hpp"

class Demuxer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList tracks READ getTracks CONSTANT)
    Q_PROPERTY(qreal audioDuration READ audioDuration CONSTANT)
    Q_PROPERTY(qreal videoDuration READ videoDuration CONSTANT)
private:
    DecodeDispatcher *m_worker = nullptr;
    QThread *m_affinityThread = nullptr;
    FrameFreeQueue m_freeQueue;
    FrameFreeFunc m_freeFunc;

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


    VideoFrame getPicture(bool b, bool own) { return m_worker->getPicture(b, own); }

    bool popPicture(bool b) { return m_worker->popPicture(b); }

    AudioFrame getSample(bool b) { return m_worker->getSample(b); }

    bool popSample(bool b) { return m_worker->popSample(b); }

    qreal audioDuration() { return m_worker ? m_worker->getAudionLength() : 0.0; }

    qreal videoDuration() { return m_worker ? m_worker->getVideoLength() : 0.0; }

    QStringList getTracks() {
        QStringList ret;
        if (m_worker) {
            auto tracks = m_worker->audioIndex();
            ret.reserve(static_cast<qsizetype>(tracks.size()));
            std::transform(tracks.begin(), tracks.end(), ret.begin(), [this](StreamIndex i){
                return m_worker->getStreamInfo(i).getFriendName();
            });
        }
        return ret;
    }


    /**
     * 向 DecodeThread 发送信号尽快暂停解码, 并唤醒阻塞在上面的线程.
     * @see DecodeDispatcher::statePause
    */
    void pause() {
        m_worker->statePause();
    }

    /**
     * 清空旧的帧, 这个方法会阻塞直到队列中的所有旧帧清理完成.
     * @see DecodeDispatcher::flush
     */
    void flush() {
        m_worker->flush();
    }

    /**
     * 在 DecodeThread 启动解码器, 这个方法是非阻塞的, 但是可以保证返回后队里请求能够被阻塞.
     */
    void start() {
        qDebug() << "Start Decoder";
        m_worker->stateResume();
        emit signalStartWorker(QPrivateSignal());
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
        try {
            m_worker = new DecodeDispatcher(fn, &m_freeQueue, &m_freeFunc, DEFAULT_STREAM_INDEX, DEFAULT_STREAM_INDEX, this);
        } catch (std::runtime_error &ex) {
            qWarning() << "Error opening file:" << ex.what();
            m_worker = nullptr;
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        connect(this, &Demuxer::signalStartWorker, m_worker, &DecodeDispatcher::onWork);
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

