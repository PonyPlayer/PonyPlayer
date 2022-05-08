//
// Created by kurisu on 2022/5/8.
//

#ifndef PONYPLAYER_PREVIEW_H
#define PONYPLAYER_PREVIEW_H
#include "dispatcher.hpp"
#include <QImage>

INCLUDE_FFMPEG_BEGIN
#include <libswscale/swscale.h>
INCLUDE_FFMPEG_END

class Previewer : public DemuxDispatcherBase {
    Q_OBJECT
private:
    int videoStreamIndex{-1};
    AVStream *videoStream{};
    DecoderContext* ctx{};
    AVPacket *pkt{};

    AVFrame *rgbFrame{};
    struct SwsContext *imgCtx{};
    unsigned char *outBuffer{};

public:
    explicit Previewer(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent) {
        for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex = i;
                videoStream = fmtCtx->streams[i];
                ctx = new DecoderContext(videoStream);
                pkt = av_packet_alloc();
                rgbFrame = av_frame_alloc();
                imgCtx = sws_getContext(ctx->codecCtx->width,
                                         ctx->codecCtx->height,
                                         ctx->codecCtx->pix_fmt,
                                         ctx->codecCtx->width,
                                         ctx->codecCtx->height,
                                         AV_PIX_FMT_RGB32,
                                         SWS_BICUBIC, nullptr,nullptr,nullptr);
                if (!imgCtx) {
                    throw std::runtime_error("Previewer: sws_getContext failed");
                }
                auto numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32,ctx->codecCtx->width,ctx->codecCtx->height,1);
                outBuffer = (unsigned char *)av_malloc(static_cast<size_t>(numBytes)*sizeof(unsigned char));
                if (av_image_fill_arrays(
                        rgbFrame->data,rgbFrame->linesize,
                        outBuffer,AV_PIX_FMT_RGB32,
                        ctx->codecCtx->width,ctx->codecCtx->height,1) < 0) {
                    throw std::runtime_error("Previewer: av_image_fill_arrays failed");
                }
                return;
            }
        }
        qWarning() << "Previewer: can not find video stream";
    }

    ~Previewer() {
        delete ctx;
        if (pkt) av_packet_free(&pkt);
        if (rgbFrame) av_frame_free(&rgbFrame);
        if (outBuffer) av_freep(&outBuffer);
        if (imgCtx) sws_freeContext(imgCtx);
    }

    /**
     * 返回pos位置的图片
     * @param pos 单位为秒
     * @return pos位置的图片
     */
    QImage previewRequest(qreal pos) {
        if (!videoStream)
            return {};
        int ret = 0;
        // 每次preview，都要先清空内部buffer，然后seek
        avcodec_flush_buffers(ctx->codecCtx);
        ret = av_seek_frame(fmtCtx, -1,
                            static_cast<int64_t>(pos * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            qWarning() << "Previewer: av_seek_frame failed";
            return {};
        }

        while (true) {
            ret = av_read_frame(fmtCtx, pkt);
            if (ret < 0) {
                qWarning() << "Previewer: reach eof, no available picture";
                break;
            }
            if (pkt->stream_index == videoStreamIndex) {
                ret = avcodec_send_packet(ctx->codecCtx, pkt);
                if (ret < 0) {
                    qWarning() << "avcodec_send_packet: " << ffmpegErrToString(ret);
                    break;
                }
                while ((ret = avcodec_receive_frame(ctx->codecCtx, ctx->frameBuf)) >= 0) {
                    double pts =  static_cast<double>(ctx->frameBuf->pts) * av_q2d(videoStream->time_base);
                    if (pts < pos) {
                        av_frame_unref(ctx->frameBuf);
                    }
                    else {
                        sws_scale(imgCtx,
                                  ctx->frameBuf->data,ctx->frameBuf->linesize,
                                  0,ctx->codecCtx->height,
                                  rgbFrame->data,rgbFrame->linesize);
                        av_frame_unref(ctx->frameBuf);

                        av_packet_unref(pkt);
                        return {outBuffer,
                                   ctx->codecCtx->width,ctx->codecCtx->height,
                                   QImage::Format_RGB32};
                    }
                }
                if (ret < 0) {
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
                    else {
                        qWarning() << "avcodec_receive_frame: " << ffmpegErrToString(ret);
                        break;
                    }
                }
            }
            av_packet_unref(pkt);
        }
        return {};
    };
};

#endif //PONYPLAYER_PREVIEW_H
