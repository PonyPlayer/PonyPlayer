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
    LifeCycleManager *m_lifeManager{};

public:

    explicit Previewer(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent) {
        for (StreamIndex i = 0; i < fmtCtx->nb_streams; ++i) {
            if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex = static_cast<int>(i);
                videoStream = fmtCtx->streams[i];
                ctx = new DecoderContext(videoStream);
                pkt = av_packet_alloc();
                m_lifeManager = new LifeCycleManager;
                return;
            }
        }
        qWarning() << "Previewer: can not find video stream";
    }

    ~Previewer() {
        delete ctx;
        if (pkt) av_packet_free(&pkt);
        if (m_lifeManager) m_lifeManager->deleteLater();
    }

    /**
     * 返回pos位置的图片
     * @param pos 单位为秒
     * @return pos位置的图片
     */
    VideoFrameRef previewRequest(qreal pos) {
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
                    } else {
                        auto *frame = ctx->frameBuf;
                        ctx->frameBuf = av_frame_alloc();
                        av_packet_unref(pkt);
//                        m_lifeManager->pop();
                        return {frame, true, pts};
                    }
                }
                if (ret < 0) {
                    if (ret == AVERROR(EAGAIN) || ret == ERROR_EOF);
                    else {
                        qWarning() << "avcodec_receive_frame: " << ffmpegErrToString(ret);
                        break;
                    }
                }
            }
            av_packet_unref(pkt);
        }
        return {};
    }
};

#endif //PONYPLAYER_PREVIEW_H
