//
// Created by kurisu on 2022/4/16.
//

#ifndef FFMPEGCMAKE_DEMUXER_H
#define FFMPEGCMAKE_DEMUXER_H

#include <helper.h>

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
INCLUDE_FFMPEG_END

#include "packet_queue.h"
#include "frame_queue.h"
#include <string>
#include <thread>
#include <optional>

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

    AVFrame *yuvFrame{};
    AVPacket *pkt{};

    PacketQueue videoPacketQueue{};
    FrameQueue videoFrameQueue{};

    std::thread workerDemuxer;

    bool isQuit{};
    bool isPause{};

    void closeCtx() {
        if (videoCodecCtx) avcodec_close(videoCodecCtx);
        if (fmtCtx) avformat_close_input(&fmtCtx);
    }

    void cleanUp() {
        closeCtx();
        if (videoCodecCtx) avcodec_free_context(&videoCodecCtx);
    }

    int initVideoState();

    void demuxer();

    void videoDecoder();

public:
    Demuxer() :
            yuvFrame(av_frame_alloc()),
            pkt(av_packet_alloc()) {}

    ~Demuxer() {
        workerDemuxer.join();
    }

    /**
     * 启动所有worker线程，当前版本需要先调用openFile打开文件
     */
    void initDemuxer();

    /**
     * 打开一个视频文件
     * @param inputFileName 文件名
     * @return 0表示成功，-1表示失败
     */
    int openFile(std::string inputFileName);

    /**
     * 获得图像数据
     * @param block 是否需要阻塞
     * @return 图像数据，根据picture.isValid()判断是否返回了可用数据，如果没有返回可用数据，
     * 可能是因为暂时没有数据（比如当前视频已经播放结束或者正常的饥饿，这种情况重试即可），
     * 也可能是因为上层下达了暂停/终止命令，需要自行判断
     */
    Picture getPicture(bool block);

    /**
     * 结束demuxer
     */
    void quit();

    /**
     * 暂停demuxer
     */
    void pause();

    /**
     * 恢复demuxer
     */
    void resume();
};

void test_saveFrame(std::string filename, int n);

void test_quit(std::string filename);

void test_pause(std::string filename, bool halfQuit);

#endif //FFMPEGCMAKE_DEMUXER_H
