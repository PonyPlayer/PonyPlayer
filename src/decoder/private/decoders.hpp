//
// Created by ColorsWind on 2022/5/18.
//
#pragma once

#include "helper.hpp"
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END
#include <QDebug>
#include "frame.hpp"
#include "twins_queue.hpp"
#include "concurrentqueue.h"
#include "audioformat.hpp"
#include <atomic>
#include <utility>

class IDemuxDecoder {

public:
    /**
     * 解码器类型
     */
    enum class DecoderType {
        Audio,  ///< 音频解码器
        Video,  ///< 视频解码器
        Common, ///< 未指定
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
     * 获取视频帧并从队列中删除, 仅当当前解码器是视频解码器时有效
     * @param b 是否阻塞
     * @return 视频帧, 请用 isValid 判断是否有效
     */
    virtual VideoFrameRef getPicture() = 0;

    /**
    * 获取音频帧并从队列中删除, 仅当当前解码器是音频解码器时有效
    * @param b 是否阻塞
    * @return 音频帧, 请用 isValid 判断是否有效
    */
    virtual AudioFrame getSample() = 0;

    /**
     * 获取队首帧的PTS, 若不存在, 返回NaN
     * @return 队首帧的PTS
     */
    virtual qreal viewFront() = 0;

    /**
     * 线性扫描移除满足条件的帧, 当发现帧不满足条件时, 结束扫描
     * @param predicate 条件
     * @return 移除帧的个数
     */
    virtual int skip(const std::function<bool(qreal)> &predicate) = 0;

    /**
     * 获取流的长度
     * @return
     */
    virtual double duration() = 0;


    virtual void setEnable(bool b) = 0;

    virtual ~IDemuxDecoder() = default;

    virtual void setFollower(IDemuxDecoder* follower) {NOT_IMPLEMENT_YET}

    virtual void pushFrameStack() {}

    virtual qreal getLastPts() {
        NOT_IMPLEMENT_YET
    }

    virtual void clearFrameStack() {}

    virtual void setStart(qreal secs) {}

    virtual qreal nextSegment() {
        NOT_IMPLEMENT_YET
    }

    virtual PonyAudioFormat getInputFormat() = 0;

    virtual void setOutputFormat(const PonyAudioFormat& format) = 0;
};

class DecoderContext {
public:
    AVCodec *codec = nullptr;
    AVStream *stream = nullptr;
    AVCodecContext *codecCtx = nullptr;
    AVFrame *frameBuf = nullptr;
    DecoderContext(AVStream *vs): stream(vs) {
        auto *videoCodecPara = stream->codecpar;
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
        if (!(frameBuf = av_frame_alloc())) {
            throw std::runtime_error("Cannot alloc frame buf.");
        }
    }

    ~DecoderContext() {
        if (frameBuf) { av_frame_free(&frameBuf); }
        if (codecCtx) { avcodec_close(codecCtx); }
        if (codecCtx) { avcodec_free_context(&codecCtx); }
    }

};


constexpr auto Audio  =  IDemuxDecoder::DecoderType::Audio;
constexpr auto Video  =  IDemuxDecoder::DecoderType::Video;
constexpr auto Common =  IDemuxDecoder::DecoderType::Common;
