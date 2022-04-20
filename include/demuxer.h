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
 *          yuvFrame = demuxer.videoFrameQueueFront();
 *          if (yuvFrame) {
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
    AVFrame *yuvFrame{};
    AVPacket *pkt{};

    PacketQueue videoPacketQueue{};
    FrameQueue videoFrameQueue{};

    std::thread workerDemuxer;

    bool isQuit{};

    void closeCtx() {
        if (videoCodecCtx) avcodec_close(videoCodecCtx);
        if (fmtCtx) avformat_close_input(&fmtCtx);
        if (imgSwsCtx) sws_freeContext(imgSwsCtx);
    }

    void cleanUp() {
        closeCtx();
        if (rgbOutBuf) av_free(rgbOutBuf);
        if (videoCodecCtx) avcodec_free_context(&videoCodecCtx);
    }

    int initVideoState();

    void demuxer();

    void videoDecoder();

public:
    Demuxer() :
            rgbFrame(av_frame_alloc()),
            yuvFrame(av_frame_alloc()),
            pkt(av_packet_alloc()) {}

    ~Demuxer() {
        workerDemuxer.join();
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
     * 获取一个视频帧，返回nullptr表示当前没有可用帧，可能是因为暂时没有
     * 也可能是因为应用已经结束，需要根据控制层是否已下达结束指令来判断
     * @return nullptr或Frame指针
     */
    Frame *videoFrameQueueFront();

    /**
     * 成功获取一个视频帧后需要调用该函数从队列中弹出帧
     */
    void videoFrameQueuePop();

    /**
     * 把从FFmpeg解码得到的YUV420图像帧转码成RGB24
     * @param src 通过videoFrameQueueFront获得的Frame
     */
    void toRGB24(Frame* src);

    /**
     * 结束demuxer，此后Front函数都会返回nullptr
     */
    void quit();
};

void test_saveFrameRGB24(std::string filename, int n);

int test_quit(std::string filename);

#endif //FFMPEGCMAKE_DEMUXER_H
