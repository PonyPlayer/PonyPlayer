//
// Created by kurisu on 2022/4/16.
//

#include "demuxer.h"

int Demuxer::initVideoState() {
    auto videoCodecPara = fmtCtx->streams[videoStreamIndex]->codecpar;
    if (!(videoCodec = (AVCodec *) avcodec_find_decoder(videoCodecPara->codec_id))) {
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

    for (int i = 0; i < fmtCtx->nb_streams; i++) {
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

    if (!(imgSwsCtx = sws_getContext(videoCodecCtx->width,
                                     videoCodecCtx->height,
                                     videoCodecCtx->pix_fmt,
                                     videoCodecCtx->width,
                                     videoCodecCtx->height,
                                     AV_PIX_FMT_RGB24,
                                     SWS_BICUBIC, NULL, NULL, NULL))) {
        printf("error get sws context\n");
        goto error;
    }

    rgbOutBufSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
                                             videoCodecCtx->width,
                                             videoCodecCtx->height,
                                             1);

    if (!(rgbOutBuf = (uint8_t *)av_malloc(rgbOutBufSize * sizeof(uint8_t)))) {
        rgbOutBufSize = 0;
        printf("error av_malloc rgb output buf\n");
        goto error;
    }

    /*
     * 该函数会把rgbOutBuf作为新格式数据的底层缓冲区，不会再动态分配
     * 因此rgbFrame不需要unref
     * */
    if (av_image_fill_arrays(
            rgbFrame->data,rgbFrame->linesize,
            rgbOutBuf,AV_PIX_FMT_RGB24,
            videoCodecCtx->width,videoCodecCtx->height,1) < 0) {
        printf("error av_image_fill_arrays\n");
        goto error;
    }

    return 0;

    error:
    closeCtx();
    return -1;
}

int Demuxer::initDemuxer() {
    workerDemuxer = std::thread(&Demuxer::demuxer, this);
}

void Demuxer::quit() {
    isQuit = true;
    videoPacketQueue.abort_request = true;
}

void Demuxer::demuxer() {
    int ret;
    auto workerVideoDecoder = std::thread(&Demuxer::videoDecoder, this);
    for (;;) {
        if (isQuit) {
            break;
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
        while ((ret = avcodec_receive_frame(videoCodecCtx, frame)) >= 0) {
            auto picFrame = videoFrameQueue.nextWritePos();

            picFrame->height = frame->height;
            picFrame->width = frame->width;
            picFrame->frameRate = av_q2d(videoStream->r_frame_rate);
            picFrame->pts = static_cast<double>(frame->pts) * av_q2d(videoStream->time_base);
            av_frame_move_ref(picFrame->frame, frame);

            av_frame_unref(frame);
            videoFrameQueue.push();
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

Frame Demuxer::toRGB24(Frame *src) {
    Frame dst = *src;
    sws_scale(imgSwsCtx,
              src->frame->data,src->frame->linesize,
              0,videoCodecCtx->height,
              rgbFrame->data,rgbFrame->linesize);
    dst.frame = rgbFrame;
    return dst;
}

Frame *Demuxer::videoFrameQueueFront() {
    return videoFrameQueue.front();
}

void Demuxer::videoFrameQueuePop() {
    videoFrameQueue.pop();
}
