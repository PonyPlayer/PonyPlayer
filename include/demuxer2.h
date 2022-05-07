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

    /**
     * 保证阻塞获取结果的线程尽快被唤醒, 同时请求 DecodeThread 的工作尽快停止.
     */
    void statePause() {
        interrupt = true;
        videoQueue->close();
        qDebug() << "Queue close.";
    }

    void flush();


    /**
     * 保证可以阻塞地获取 Picture 和 Sample. 这个方法是线程安全的.
     */
    void stateResume() {
        videoQueue->open();
        qDebug() << "Queue stateResume.";
    }

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


#endif //PONYPLAYER_DEMUXER2_H
