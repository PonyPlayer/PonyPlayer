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

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END


inline int ffmpegErrToString(int err) {
    char errBuf[512];
    av_strerror(err, errBuf, sizeof(errBuf));
    return 0;
}


class DemuxDispatcherBase: public QObject {
Q_OBJECT
public:
    const std::string filename;
protected:

    AVFormatContext *fmtCtx = nullptr;

    explicit DemuxDispatcherBase(const std::string &fn): filename(fn) {
        if (avformat_open_input(&fmtCtx, fn.c_str(), nullptr, nullptr) < 0) {
            throw std::runtime_error("Cannot open input file.");
        }
        if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
            throw std::runtime_error("Cannot find any stream in file.");
        }
    }
    ~DemuxDispatcherBase() override {
        if (fmtCtx) { avformat_close_input(&fmtCtx); }
    }
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

    virtual BlockingQueue<AVFrame *>& getFrameQueue() = 0;

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
    BlockingQueue<AVFrame *> frameQueue;
    AVFrame *frameBuf = nullptr;

    // Audio only
    SwrContext *swrCtx = nullptr;
    uint8_t *audioOutBuf = nullptr;
    AVFrame * sampleFrameBuf = nullptr;

public:
    DecoderImpl(AVStream *vs, long long int capacity) : stream(vs), frameQueue(capacity) {
        auto videoCodecPara = stream->codecpar;
        if (!(codec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecPara->codec_id)))) {
            throw std::runtime_error("Cannot find valid video decode codec.");
        }
        if (!(codecCtx = avcodec_alloc_context3(codec))) {
            throw std::runtime_error("Cannot find valid video decode codec context.");
        }
        if (avcodec_parameters_to_context(codecCtx, videoCodecPara) < 0) {
            throw std::runtime_error("Cannot initialize videoCodecCtx.");
        }
        if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
            throw std::runtime_error("Cannot open codec.");
        }

        frameBuf = av_frame_alloc();
        if constexpr(type == DemuxDecoder::DecoderType::Audio) {
            this->swrCtx = swr_alloc_set_opts(swrCtx, av_get_default_channel_layout(2),
                                              AV_SAMPLE_FMT_S16, 44100,
                                              static_cast<int64_t>(codecCtx->channel_layout), codecCtx->sample_fmt,
                                              codecCtx->sample_rate, 0, nullptr);

            if (!swrCtx || swr_init(swrCtx) < 0) {
                throw std::runtime_error("Cannot initialize swrCtx");
            }
            if (!(audioOutBuf = static_cast<uint8_t *>(av_malloc(2 * MAX_AUDIO_FRAME_SIZE)))) {
                throw std::runtime_error("Cannot alloc audioOutBuf");
            }
            sampleFrameBuf = av_frame_alloc();
        }
    }

    ~DecoderImpl() override {
        if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
        if (audioOutBuf) { av_freep(audioOutBuf); }
        if (swrCtx) { swr_free(&swrCtx); }
        if (frameBuf) { av_frame_free(&frameBuf); }
        if (codecCtx) { avcodec_close(codecCtx); }
        if (codecCtx) { avcodec_free_context(&codecCtx); }
    }

    qreal duration() override {
        return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }

    bool accept(AVPacket *pkt, std::atomic<bool> &interrupt) override {
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0 && !interrupt) {
            ret = avcodec_receive_frame(codecCtx, frameBuf);
            if (ret >= 0) {
                ret = frameQueue.bpush(frameBuf);
                frameBuf = av_frame_alloc();
            } else if (ret == AVERROR(EAGAIN)) {
                return true;
            } else if (ret == ERROR_EOF) {
                frameQueue.bpush(nullptr);
                return false;
            } else {
                frameQueue.bpush(nullptr);
                qWarning() << "Error avcodec_receive_frame:" << ffmpegErrToString(ret);
                return false;
            }
        }
        return false;
    }

    void flushFFmpegBuffers() override {
        avcodec_flush_buffers(codecCtx);
    }


    BlockingQueue<AVFrame *> &getFrameQueue() override {
        return frameQueue;
    }


    Picture getPicture(bool b) override {
        if constexpr(type != DemuxDecoder::DecoderType::Video) { throw std::runtime_error("Unsupported operation."); }
        AVFrame *frame = frameQueue.bfront();
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, pts};
    }

    Sample getSample(bool b) override {
        if constexpr(type != DemuxDecoder::DecoderType::Audio) { throw std::runtime_error("Unsupported operation."); }
        AVFrame *frame = frameQueue.bfront();
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                              const_cast<const uint8_t **>(frame->data), frame->nb_samples);

        int out_size = av_samples_get_buffer_size(0, 2,
                                                  len,
                                                  AV_SAMPLE_FMT_S16,
                                                  1);
        return {audioOutBuf, out_size, pts, frame};
    }


    bool pop(bool b) override {
        return frameQueue.bpop();
    }
};

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
public:
    explicit DecodeDispatcher(const std::string &fn) : DemuxDispatcherBase(fn) {
        packet = av_packet_alloc();
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex.emplace_back(i);
            } else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audioStreamIndex.emplace_back(i);
            }
        }
        if (videoStreamIndex.empty()) { throw std::runtime_error("Cannot find video stream."); }
        if (audioStreamIndex.empty()) { throw std::runtime_error("Cannot find audio stream."); }
        decoders.resize(fmtCtx->nb_streams);
        using DemuxDecoder::DecoderType::Audio;
        using DemuxDecoder::DecoderType::Video;
        // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
        decoders[audioStreamIndex.front()] = new DecoderImpl<Audio>(fmtCtx->streams[audioStreamIndex.front()], 256);
        decoders[videoStreamIndex.front()] = new DecoderImpl<Video>(fmtCtx->streams[videoStreamIndex.front()], 32);
        interrupt = false; // happens before
    }

    ~DecodeDispatcher() {
        qDebug() << "Destroy decode dispatcher " << filename.c_str();
        for(auto && decoder : decoders) { delete decoder; }
        decoders.clear();
        if(packet) { av_packet_free(&packet); }
    }

    void pause() {
        interrupt = true;
        for (auto && decoder : decoders) {
            if (decoder) {
                decoder->getFrameQueue().notify();
            }
        }
    }

    void flush() {
        for (auto && decoder : decoders) {
            if (decoder) {
                decoder->getFrameQueue().clearWith([](AVFrame *frame){ av_frame_free(&frame);});
            }
        }
    }

    /**
     * 修改视频播放进度, 注意: 这个方法必须在解码线程上调用.
     * @param us 新的视频进度(单位: 微秒)
     */
    void seek(int64_t us) {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        for (const auto & decoder : decoders) {
            // interrupt
            if (decoder) decoder->flushFFmpegBuffers();
        }
        qDebug() << "a Seek:" << us;
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(static_cast<double>(us) / 1000000.0 * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }

    }

    Picture getPicture(bool b) { return decoders[videoStreamIndex.front()]->getPicture(b); }

    bool popPicture(bool b) { return decoders[videoStreamIndex.front()]->pop(b); }

    Sample getSample(bool b) { return decoders[audioStreamIndex.front()]->getSample(b); }

    bool popSample(bool b) { return decoders[audioStreamIndex.front()]->pop(b); }

    qreal getAudionLength() { return decoders[audioStreamIndex.front()]->duration(); }
    qreal getVideoLength() { return decoders[videoStreamIndex.front()]->duration(); }
public slots:
    void onWork() {
        interrupt = false;
        while(!interrupt) {
            int ret = av_read_frame(fmtCtx, packet);
            if (ret == 0) {
                auto *decoder = decoders[static_cast<unsigned long>(packet->stream_index)];
                if (decoder) {
                    ret = decoder->accept(packet, interrupt);
                    av_packet_unref(packet);
                    if (!ret) {
                        // no more packet
                        break;
                    }
                } else { av_packet_unref(packet); }
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            QCoreApplication::processEvents();
        }
        interrupt = true;
    }
};

/**
 * @brief 视频解码器
 * 这个类不是RAII的
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
    Demuxer2() {
        qThread = new QThread();
        qThread->setObjectName("DecoderThread");
        qThread->start();
        this->moveToThread(qThread);
    }

    ~Demuxer2() {
        if (worker) {
            worker->pause();
            worker->deleteLater();
            worker = nullptr;
        }
        qThread->deleteLater();
    }

    Picture getPicture(bool b) { return worker->getPicture(b); }

    bool popPicture(bool b) { return worker->popPicture(b); }

    Sample getSample(bool b) { return worker->getSample(b); }

    bool popSample(bool b) { return worker->popSample(b); }

    qreal audioDuration() { return worker ? worker->getAudionLength() : 0.0 ; }

    qreal videoDuration() { return worker ? worker->getVideoLength() : 0.0; }

    /**
     * 暂停编码并使解码器线程进入空闲状态, 这个方法是线程安全的.
    */
    void interrupt() {
        worker->pause();
    }

    /**
     * 清空旧的帧, 这个方法是线程安全的.
     */
    void flush() {
        worker->flush();
    }
public slots:
    /**
     * 调整视频进度, 必须保证解码器线程空闲且缓冲区为空.
     * @param us 视频进度(单位: 微秒)
     * @see Demuxer2::interrupt
     * @see Demuxer2::flush
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
            worker = new DecodeDispatcher(fn);
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
        qDebug() << "Try Start";
        emit startWorker(QPrivateSignal());
    }




};



#endif //PONYPLAYER_DEMUXER2_H
