//
// Created by kurisu on 2022/4/16.
//

#ifndef FFMPEGCMAKE_DEMUXER_H
#define FFMPEGCMAKE_DEMUXER_H

#include <helper.h>

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
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
    const int MAX_AUDIO_FRAME_SIZE = 192000;
    std::string filename{};
    AVFormatContext *fmtCtx{};
    AVPacket *pkt{};

    int videoStreamIndex{-1};
    AVFrame *yuvFrame{};
    AVStream *videoStream{};
    AVCodec *videoCodec{};
    AVCodecContext *videoCodecCtx{};

    PacketQueue videoPacketQueue;
    FrameQueue videoFrameQueue;

    int audioStreamIndex{-1};
    AVFrame *sampleFrame{};
    AVStream *audioStream{};
    AVCodec *audioCodec{};
    AVCodecContext *audioCodecCtx{};
    SwrContext *swrCtx{};
    uint8_t *audioOutBuf{};

    PacketQueue audioPacketQueue{};
    FrameQueue audioFrameQueue{};

    std::thread workerDemuxer;

    bool isQuit{};
    bool isPause{};
    bool isEof{true};

    bool needFlush{};
    int flushFinish{};

    void closeCtx() {
        if (videoCodecCtx) {
            avcodec_flush_buffers(videoCodecCtx);
            avcodec_close(videoCodecCtx);
        }
        if (audioCodecCtx) {
            avcodec_flush_buffers(audioCodecCtx);
            avcodec_close(audioCodecCtx);
        }
        if (fmtCtx) avformat_close_input(&fmtCtx);
    }

    void cleanUp() {
        videoFrameQueue.flush();
        audioFrameQueue.flush();
        videoPacketQueue.flush();
        audioPacketQueue.flush();
        closeCtx();
        if (videoCodecCtx) avcodec_free_context(&videoCodecCtx);
        if (audioCodecCtx) avcodec_free_context(&audioCodecCtx);
    }

    bool tooManyPackets() {
        return videoFrameQueue.queue.size() >= 200 || videoFrameQueue.queue.size() >= 200;
    }

    void demuxer();

    int initVideoState();

    void videoDecoder();

    int initAudioState();

    void audioDecoder();

    void decoder(AVCodecContext **ctx, PacketQueue &pq, FrameQueue &fq, AVFrame *frame);

    void initDemuxer();

    int openFile();

public:
    Demuxer() :
            pkt(av_packet_alloc()),
            yuvFrame(av_frame_alloc()),
            sampleFrame(av_frame_alloc()) {
        initDemuxer();
    }

    ~Demuxer() {
        workerDemuxer.join();
    }

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
     * 成功获取图像数据后用于从队列中弹出一个图像帧
     */
    void popPicture();

    /**
     * 获取采样数据
     * @param block 是否需要阻塞
     * @return 一个帧的采样数据，根据sample.isValid()判断是否返回了可用数据
     */
    Sample getSample(bool block);

    /**
     * 获得一个可用的采样数据之后调用该函数弹出一个采样帧
     */
    void popSample();

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

void test_savePCM(std::string filename);

void test_audio(std::string filename);

void test_eof(std::string filename);


#endif //FFMPEGCMAKE_DEMUXER_H
