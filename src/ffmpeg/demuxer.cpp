//
// Created by kurisu on 2022/4/16.
//

#include "demuxer.h"

int Demuxer::initVideoState() {
    auto videoCodecPara = fmtCtx->streams[videoStreamIndex]->codecpar;
    if (!(videoCodec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecPara->codec_id)))) {
        printf("Cannot find valid decode codec.\n");
        return -1;
    }
    if (!(videoCodecCtx = avcodec_alloc_context3(videoCodec))) {
        printf("Cannot find valid decode codec context.\n");
        return -1;
    }
    if (avcodec_parameters_to_context(videoCodecCtx, videoCodecPara) < 0) {
        printf("Cannot initialize videoCodecCtx.\n");
        return -1;
    }
    if (avcodec_open2(videoCodecCtx, videoCodec, nullptr) < 0) {
        printf("Cannot open codec.\n");
        return -1;
    }

    videoStream = fmtCtx->streams[videoStreamIndex];
    return 0;
}

int Demuxer::openFile(std::string inputFileName) {
    if (inputFileName.empty())
        goto error;

    filename = inputFileName;

    if (avformat_open_input(&fmtCtx, filename.c_str(), nullptr, nullptr) < 0) {
        printf("Cannot open input file.\n");
        goto error;
    }

    if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
        printf("Cannot find any stream in file.\n");
        goto error;
    }

    for (int i = 0; i < static_cast<int>(fmtCtx->nb_streams); i++) {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        printf("Cannot find video stream in file.\n");
        goto error;
    }

    if (initVideoState() == -1) {
        printf("error init video state\n");
        goto error;
    }

    return 0;

    error:
    closeCtx();
    return -1;
}

void Demuxer::initDemuxer() {
    workerDemuxer = std::thread(&Demuxer::demuxer, this);
}

void Demuxer::quit() {
    isQuit = true;
    videoPacketQueue.isQuit = true;
    videoFrameQueue.isQuit = true;
}

void Demuxer::pause() {
    isPause = true;
}

void Demuxer::resume() {
    isPause = false;
}

void Demuxer::demuxer() {
    int ret;
    auto workerVideoDecoder = std::thread(&Demuxer::videoDecoder, this);
    for (;;) {
        if (isQuit) {
            break;
        }
        if (isPause) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        if (tooManyPackets()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        ret = av_read_frame(fmtCtx, pkt);
        if (ret < 0) {
            AVPacket *nullPkt = av_packet_alloc();
            nullPkt->data = nullptr;
            nullPkt->size = 0;
            videoPacketQueue.push(pkt);
            av_packet_free(&nullPkt);
            break;
        }
        if (pkt->stream_index == videoStreamIndex) {
            videoPacketQueue.push(pkt);
        } else {
            av_packet_unref(pkt);
        }
    }
    workerVideoDecoder.join();
}

void Demuxer::videoDecoder() {
    int ret;
    for (;;) {
        if (isPause && !isQuit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        AVPacket front;
        ret = videoPacketQueue.pop(&front);
        if (ret < 0) {
            break;
        }
        ret = avcodec_send_packet(videoCodecCtx, &front);
        if (ret < 0) {
            char errStr[256];
            av_strerror(ret, errStr, sizeof(errStr));
            printf("avcodec_send_packet %d %s\n", ret, errStr);
            break;
        }
        while ((ret = avcodec_receive_frame(videoCodecCtx, yuvFrame)) >= 0) {
            auto picFrame = av_frame_alloc();
            av_frame_move_ref(picFrame, yuvFrame);
            av_frame_unref(yuvFrame);
            videoFrameQueue.push(picFrame);
        }
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN))
                continue;
            else
                break;
        }
        av_packet_unref(&front);
    }
}

Picture Demuxer::getPicture(bool block) {
    Picture res;
    while (true) {
        if (isQuit) {
            break;
        }
        auto frame = videoFrameQueue.front(block);
        if (frame) {
            double pts = static_cast<double>(frame->pts) * av_q2d(videoStream->time_base);
            res = Picture(frame, pts);
            break;
        } else if (!block)
            break;
    }
    return res;
}

void Demuxer::popPicture() {
    videoFrameQueue.pop();
}