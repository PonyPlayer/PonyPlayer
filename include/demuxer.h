//
// Created by kurisu on 2022/4/16.
//

#ifndef FFMPEGCMAKE_DEMUXER_H
#define FFMPEGCMAKE_DEMUXER_H

#include <helper.h>

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

#include "packet_queue.h"
#include "frame_queue.h"
#include <string>
#include <thread>

/**
 * usecase：
 *      Demuxer demuxer;
 *      ret = demuxer.openFile(filename);
 *      initDemuxer();
 *      for (;;) {
 *          frame = demuxer.videoFrameQueueFront();
 *          if (frame) {
 *              // do something
 *              demuxer.videoFramePop();
 *          }
 *      }
 */
class Demuxer {
private:
    std::string filename{};
    AVFormatContext *fmtCtx{};

    int videoStreamIndex{-1};
    AVStream *videoStream{};
    AVCodec *videoCodec{};
    AVCodecContext *videoCodecCtx{};
    SwsContext *imgSwsCtx{};
    uint8_t *rgbOutBuf{};
    int rgbOutBufSize{};

    AVFrame *rgbFrame{};
    AVFrame *frame{};
    AVPacket *pkt{};

    PacketQueue videoPacketQueue{};
    FrameQueue videoFrameQueue{};

    std::thread workerDemuxer;

    void closeCtx() {
        if (videoCodecCtx) avcodec_close(videoCodecCtx);
        if (fmtCtx) avformat_close_input(&fmtCtx);
        if (imgSwsCtx) sws_freeContext(imgSwsCtx);
    }

    void destroy() {
        closeCtx();
        if (pkt) av_packet_free(&pkt);
        if (frame) av_frame_free(&frame);
        if (rgbFrame) av_frame_free(&rgbFrame);
        if (rgbOutBuf) av_free(rgbOutBuf);
        if (videoCodecCtx) avcodec_free_context(&videoCodecCtx);
    }

    int initVideoState();

    void demuxer();

    void videoDecoder();

public:
    Demuxer() :
            rgbFrame(av_frame_alloc()),
            frame(av_frame_alloc()),
            pkt(av_packet_alloc()) {}

    ~Demuxer() {
        destroy();
    }

    /**
     * 启动所有worker线程，当前版本需要先调用openFile打开文件
     * @return 0表示成功，-1表示失败
     */
    int initDemuxer();

    /**
     * 打开一个视频文件
     * @param inputFileName 文件名
     * @return 0表示成功，-1表示失败
     */
    int openFile(std::string inputFileName);

    /**
     * 阻塞地获取一个视频帧
     * @return 帧的指针
     */
    Frame *videoFrameQueueFront();

    /**
     * 成功获取一个视频帧后需要调用该函数从队列中弹出帧
     */
    void videoFrameQueuePop();

    Frame toRGB24(Frame* src);
};

void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);

void test_saveFrame(std::string filename);

int test_countFrame(std::string filename);

#endif //FFMPEGCMAKE_DEMUXER_H
