//
// Created by ColorsWind on 2022/4/30.
// Adapted from demuxer v1 by kurisu on 2022/4/16.
//
#pragma once
#ifndef PONYPLAYER_DEMUXER2_H
#define PONYPLAYER_DEMUXER2_H
#include <QtCore>
#include <QTimer>
#include "helper.h"
#include "blocking_queue.h"
#include "frame_queue.h"
#include "twins_queue.h"

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

//#define IGNORE_VIDEO_FRAME

inline QString ffmpegErrToString(int err) {
    char errBuf[512];
    av_strerror(err, errBuf, sizeof(errBuf));
    return errBuf;
}


class DemuxDispatcherBase: public QObject {
    Q_OBJECT
public:
    const std::string filename;
protected:

    AVFormatContext *fmtCtx = nullptr;

    explicit DemuxDispatcherBase(const std::string &fn, QObject *parent);
    ~DemuxDispatcherBase() override;
};



class DemuxDecoder {

public:
    /**
     * 解码器类型
     */
    enum class DecoderType {
        Audio, ///< 音频解码器
        Video, ///< 视频解码器
    };
    /**
     * 接收一个包
     * @param pkt
     * @return 如果还需要接收下一个 packet 返回 true, 否则返回 false
     */
    virtual bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) = 0;

    /**
     * 清空 FFmpeg 内部缓冲区
     */
    virtual void flushFFmpegBuffers() = 0;


    /**
     * 获取视频帧, 仅当当前解码器是视频解码器时有效
     * @param b 是否阻塞
     * @return 视频帧, 请用 isValid 判断是否有效
     */
    virtual Picture getPicture(bool b) = 0;

    /**
    * 获取音频帧, 仅当当前解码器是音频解码器时有效
    * @param b 是否阻塞
    * @return 音频帧, 请用 isValid 判断是否有效
    */
    virtual Sample getSample(bool b) = 0;

    /**
     * 从队列中删除视频帧/音频帧
     * @param b 是否阻塞
     * @return 是否成功
     */
    virtual bool pop(bool b) = 0;

    /**
     * 获取流的长度
     * @return
     */
    virtual qreal duration() = 0;

    virtual ~DemuxDecoder() = default;

};


/**
 * @brief 音视频解码器具体实现
 * @tparam type 解码器类型
 */
template<DemuxDecoder::DecoderType type>
class DecoderImpl : public DemuxDecoder {
private:
    AVCodec *codec = nullptr;
    AVStream *stream = nullptr;
    AVCodecContext *codecCtx = nullptr;
    TwinsBlockQueue<AVFrame *> *frameQueue;
    AVFrame *frameBuf = nullptr;

    // Audio only
    SwrContext *swrCtx = nullptr;
    uint8_t *audioOutBuf = nullptr;
    AVFrame * sampleFrameBuf = nullptr;

public:
    DecoderImpl(AVStream *vs, TwinsBlockQueue<AVFrame *> *frameQueue);

    ~DecoderImpl() override;

    qreal duration() override;

    bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override;

    void flushFFmpegBuffers() override;

    Picture getPicture(bool b) override;

    Sample getSample(bool b) override;

    bool pop(bool b) override;
};


using DemuxDecoder::DecoderType::Audio;
using DemuxDecoder::DecoderType::Video;


/**
 * @brief 解码器调度器, 将Packet分配给解码器进一步解码成Frame
 * 这个类是RAII的
 */
class DecodeDispatcher : public DemuxDispatcherBase {
    Q_OBJECT
    std::vector<unsigned int> videoStreamIndex;
    std::vector<unsigned int> audioStreamIndex;
    std::vector<DemuxDecoder*> decoders;
    std::atomic<bool> interrupt = false;
    AVPacket *packet = nullptr;
    TwinsBlockQueue<AVFrame *> *videoQueue;
    TwinsBlockQueue<AVFrame *> *audioQueue;
public:
    explicit DecodeDispatcher(const std::string &fn, QObject *parent);

    ~DecodeDispatcher() override;

    void pause() {
        interrupt = true;
        videoQueue->close();
    }

    void flush();

    /**
     * 修改视频播放进度, 注意: 这个方法必须在解码线程上调用.
     * @param us 新的视频进度(单位: 微秒)
     */
    void seek(int64_t us);

    Picture getPicture(bool b) { return decoders[videoStreamIndex.front()]->getPicture(b); }

    bool popPicture(bool b) { return decoders[videoStreamIndex.front()]->pop(b); }

    Sample getSample(bool b) { return decoders[audioStreamIndex.front()]->getSample(b); }

    bool popSample(bool b) { return decoders[audioStreamIndex.front()]->pop(b); }

    qreal getAudionLength() { return decoders[audioStreamIndex.front()]->duration(); }
    qreal getVideoLength() { return decoders[videoStreamIndex.front()]->duration(); }
public slots:
    void onWork();
};

/**
 * @brief 视频解码器, 负责从文件中读取视频帧和音频帧.
 *
 * 生命周期与打开文件相同. 这个类是RAII的.
 */
class Demuxer2: public QObject {
    Q_OBJECT
private:
    DecodeDispatcher *worker = nullptr;
    QThread *qThread = nullptr;
signals:
    void startWorker(QPrivateSignal);
    void openFileResult(bool ret, QPrivateSignal);
public:
    Demuxer2(QObject *parent) : QObject(nullptr) {
        qThread = new QThread(parent);
        qThread->setObjectName("DecoderThread");
        qThread->start();
        this->moveToThread(qThread);
    }

    ~Demuxer2() {
        qDebug() << "Destroy Demuxer2";
        if (worker) {
            worker->pause();
            worker->deleteLater();
        }
        qThread->quit();
    }

    Picture getPicture(bool b) { return worker->getPicture(b); }

    bool popPicture(bool b) { return worker->popPicture(b); }

    Sample getSample(bool b) { return worker->getSample(b); }

    bool popSample(bool b) { return worker->popSample(b); }

    qreal audioDuration() { return worker ? worker->getAudionLength() : 0.0 ; }

    qreal videoDuration() { return worker ? worker->getVideoLength() : 0.0; }

    /**
     * 暂停编码并使解码器线程进入空闲状态, 这个方法是线程安全的.
     * @see DecodeDispatcher::pause
    */
    void pause() {
        worker->pause();
    }

    /**
     * 清空旧的帧, 这个方法必须在 VideoThread 上调用.
     * @see DecodeDispatcher::flush
     */
    void flush() {
        worker->flush();
    }
public slots:
    /**
     * 调整视频进度, 必须保证解码器线程空闲且缓冲区为空. \n
     * 一次完整的调整进度操作应该为: \n
     * 1. 在VideoThread线程调用 Demuxer2::pause 使解码器线程停止运行; \n
     * 2. 在VideoThread线程调用 Demuxer2::seek 并阻塞等待函数返回, 接下来产生的帧是新的帧. \n
     * 3. 在VideoThread线程调用 Demuxer2::flush 清空队列中的旧帧. \n
     * 4. 在DeocdeThread线程中执行 Demuxer2::start, 恢复解码器线程线程运行. \n
     * @param us 视频进度(单位: 微秒)
     * @see Demuxer2::pause
     * @see Demuxer2::flush
     * @see DecodeDispatcher::seek
     */
    void seek(int64_t us) {
        worker->seek(us);
    };

    /**
     * 打开文件
     * @param fn 本地文件路径
     */
    void openFile(const std::string &fn) {
        qDebug() << "Open file" << QString::fromUtf8(fn);
        // call on video decoder thread
        if (worker) {
            qWarning() << "Already open file:" << QString::fromUtf8(worker->filename);
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        try {
            worker = new DecodeDispatcher(fn, this);
        } catch (std::runtime_error &ex) {
            qWarning() << "Error opening file:" << ex.what();
            worker = nullptr;
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        connect(this, &Demuxer2::startWorker, worker, &DecodeDispatcher::onWork);
        emit openFileResult(true, QPrivateSignal());
    }

    void close() {
        if (worker) {
            qDebug() << "Close file" << worker->filename.c_str();
            worker->pause();
            worker->deleteLater();
            worker = nullptr;
        } else {
            qWarning() << "Try to close file while no file has been opened.";
        }
    }

    /**
     * 启动解码器
     */
    void start() {
        qDebug() << "Start Decoder";
        emit startWorker(QPrivateSignal());
    }




};



#endif //PONYPLAYER_DEMUXER2_H
