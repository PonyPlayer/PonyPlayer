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

class DemuxWorkerBase: public QObject {
Q_OBJECT
public:
    const std::string filename;
protected:

    AVFormatContext *fmtCtx = nullptr;

    DemuxWorkerBase(const std::string &fn): filename(fn) {
        if (avformat_open_input(&fmtCtx, fn.c_str(), nullptr, nullptr) < 0) {
            throw std::runtime_error("Cannot open input file.");
        }
        if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
            throw std::runtime_error("Cannot find any stream in file.");
        }
    }
    virtual ~DemuxWorkerBase() {
        if (fmtCtx) { avformat_close_input(&fmtCtx); }
    }
};

class DemuxMetaDataWorker : public DemuxWorkerBase {
    std::vector<unsigned int> videoStreamIndex;
    std::vector<unsigned int> audioStreamIndex;

public:
    void load() {
        // find stream
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex.emplace_back(i);
            } else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audioStreamIndex.emplace_back(i);
            }
        }
    }

};

class DecoderBase {

public:
    enum class DecoderType {
        Audio, Video
    };
    /**
     * 接收一个包
     * @param pkt
     * @return 如果还需要接收下一个 packet 返回 true, 否则返回 false
     */
    virtual bool accept(AVPacket *pkt) = 0;


    virtual void beforeSeek(int64_t us) = 0;

    virtual void afterSeek() = 0;

    virtual Picture getPicture(bool b) = 0;

    virtual Sample getSample(bool b) = 0;

    virtual bool pop(bool b) = 0;

    virtual qreal duration() = 0;

    virtual ~DecoderBase() = default;


};


template<DecoderBase::DecoderType type>
class DecoderImpl : public DecoderBase {
private:
    AVCodec *codec = nullptr;
    AVStream *stream = nullptr;
    AVCodecContext *codecCtx = nullptr;
    BlockingQueue<AVFrame *> frameQueue;
    AVFrame *frameBuf = nullptr;



    // Audio only
    const int MAX_AUDIO_FRAME_SIZE = 192000;
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
        if constexpr(type == DecoderBase::DecoderType::Audio) {
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

    ~DecoderImpl() {
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

    bool accept(AVPacket *pkt) override {
        int ret = avcodec_send_packet(codecCtx, pkt);
        if (ret < 0) {
            qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
            return false;
        }
        while(ret >= 0) {
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

    void beforeSeek(int64_t us) override {
        frameQueue.clearWith([](AVFrame * frame) { av_frame_free(&frame);});
        avcodec_flush_buffers(codecCtx);
        frameQueue.notify();
    }

    void afterSeek() override {

    }

    Picture getPicture(bool b) {
        if constexpr(type != DecoderBase::DecoderType::Video) { throw std::runtime_error("Unsupported operation."); }
        AVFrame *frame = frameQueue.bfront();
        if (!frame) { return {}; }
        double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
        return {frame, pts};
    }

    Sample getSample(bool b) {
        if constexpr(type != DecoderBase::DecoderType::Audio) { throw std::runtime_error("Unsupported operation."); }
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

class DecodeWorker : public DemuxWorkerBase {
    Q_OBJECT
    std::vector<unsigned int> videoStreamIndex;
    std::vector<unsigned int> audioStreamIndex;
    std::vector<DecoderBase*> decoders;
    std::atomic<bool> interrupt = false;
    AVPacket *packet = nullptr;
public:
    DecodeWorker(const std::string &fn) : DemuxWorkerBase(fn) {
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
        using DecoderBase::DecoderType::Audio;
        using DecoderBase::DecoderType::Video;
        decoders[audioStreamIndex.front()] = new DecoderImpl<Audio>(fmtCtx->streams[audioStreamIndex.front()], 2048);
        decoders[videoStreamIndex.front()] = new DecoderImpl<Video>(fmtCtx->streams[videoStreamIndex.front()], 512);
    }

    ~DecodeWorker() {
        for(auto && decoder : decoders) { delete decoder; }
        if(packet) { av_packet_free(&packet); }
        DemuxWorkerBase::~DemuxWorkerBase();
    }

    /**
     * 修改视频播放进度, 注意: 这个方法必须不在解码线程上调用.
     * @param us 新的视频进度(单位: 微秒)
     */
    void seek(int64_t us) {
        // case 1: currently decoding, interrupt
        // case 2: not decoding, seek
        interrupt = true;
        for (const auto & decoder : decoders) {
            // interrupt
            if (decoder) decoder->beforeSeek(us);
        }
        qDebug() << "a Seek:" << us;
        int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(static_cast<double>(us) / 1000000.0 * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret == 0) {
            for (const auto & decoder : decoders) {
                if (decoder) decoder->afterSeek();
            }
        } else {
            qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret);
        }

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
                    if (decoder->accept(packet)) {
                        av_packet_unref(packet);
                    } else {
                        break;
                        // no more frame
                    }
                }
            } else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            QCoreApplication::processEvents();
        }
        interrupt = true;
    }
};


class Demuxer2: public QObject {
    Q_OBJECT
private:
    DecodeWorker *worker = nullptr;
    QThread *qThread = nullptr;
signals:
    void startWorker(QPrivateSignal);
    void seekWorker(QPrivateSignal);
    void openFileResult(bool ret, QPrivateSignal);
    void seekCompleted(int64_t pos, QPrivateSignal);
public:
    Demuxer2() {
        qThread = new QThread();
        qThread->setObjectName("DecoderThread");
        qThread->start();
    }

    void move() {
        this->moveToThread(qThread);
    }

    ~Demuxer2() {}

    Picture getPicture(bool b) { return worker->getPicture(b); }

    bool popPicture(bool b) { return worker->popPicture(b); }

    Sample getSample(bool b) { return worker->getSample(b); }

    bool popSample(bool b) { return worker->popSample(b); }

    qreal audioDuration() { return worker ? worker->getAudionLength() : 0.0 ; }

    qreal videoDuration() { return worker ? worker->getVideoLength() : 0.0; }

public slots:
    /**
     * 调整视频进度
     * @param us 视频进度(单位: 微秒)
     */
    void seek(int64_t us) {
        // call on DecodeThread
        worker->seek(us);
//        emit seekCompleted(us, QPrivateSignal());
//        emit startWorker(QPrivateSignal());
    };

    /**
     * 打开文件
     * @param fn 本地文件路径
     */
    void openFile(const std::string &fn) {
        qDebug() << "OpenFile" << QString::fromUtf8(fn);
        // call on video decoder thread
        if (worker) {
            qWarning() << "Already open file:" << QString::fromUtf8(worker->filename);
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        try {
            worker = new DecodeWorker(fn);
        } catch (std::runtime_error &ex) {
            qWarning() << "Error opening file:" << ex.what();
            worker = nullptr;
            emit openFileResult(false, QPrivateSignal());
            return;
        }
        connect(this, &Demuxer2::startWorker, worker, &DecodeWorker::onWork);
        emit openFileResult(true, QPrivateSignal());
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
