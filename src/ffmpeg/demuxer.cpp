//
// Created by kurisu on 2022/4/16.
//

#include "demuxer.h"
#include <QDebug>

void debugErr(std::string prefix, int err) {
    char errBuf[512] = {0};
    av_strerror(err, errBuf, sizeof(errBuf));
    qWarning("%s: %s\n", prefix.c_str(), errBuf);
}

int Demuxer::initVideoState() {
    auto videoCodecPara = fmtCtx->streams[videoStreamIndex]->codecpar;
    if (!(videoCodec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecPara->codec_id)))) {
        qWarning("Cannot find valid video decode codec.");
        return -1;
    }
    if (!(videoCodecCtx = avcodec_alloc_context3(videoCodec))) {
        qWarning("Cannot find valid video decode codec context.");
        return -1;
    }
    if (avcodec_parameters_to_context(videoCodecCtx, videoCodecPara) < 0) {
        qWarning("Cannot initialize videoCodecCtx.");
        return -1;
    }
    if (avcodec_open2(videoCodecCtx, videoCodec, nullptr) < 0) {
        qWarning("Cannot open codec.\n");
        return -1;
    }

    videoStream = fmtCtx->streams[videoStreamIndex];
    return 0;
}

int Demuxer::initAudioState() {
    auto audioCodecPara = fmtCtx->streams[audioStreamIndex]->codecpar;
    if (!(audioCodec = const_cast<AVCodec *>(avcodec_find_decoder(audioCodecPara->codec_id)))) {
        qWarning("Cannot find valid audio decode codec.\n");
        return -1;
    }
    if (!(audioCodecCtx = avcodec_alloc_context3(audioCodec))) {
        qWarning("Cannot find valid audio decode codec context.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(audioCodecCtx, audioCodecPara) < 0) {
        qWarning("Cannot initialize audioCodecCtx.\n");
        return -1;
    }

    if (avcodec_open2(audioCodecCtx, audioCodec, nullptr) < 0) {
        qWarning("Cannot open codec.\n");
        return -1;
    }

    audioStream = fmtCtx->streams[audioStreamIndex];

    swrCtx = swr_alloc_set_opts(swrCtx, av_get_default_channel_layout(2),
                                AV_SAMPLE_FMT_S16, 44100,
                                audioCodecCtx->channel_layout, audioCodecCtx->sample_fmt,
                                audioCodecCtx->sample_rate, 0, nullptr);

    if (!swrCtx) {
        qWarning("can not initialize swrCtx\n");
        return -1;
    }

    int ret = swr_init(swrCtx);
    if (ret < 0) {
        debugErr("swr_init", ret);
        return -1;
    }

    if (!(audioOutBuf = static_cast<uint8_t *>(av_malloc(2 * MAX_AUDIO_FRAME_SIZE)))) {
        qWarning("oom: can not alloc audioOutBuf\n");
        return -1;
    }

    return 0;
}

int Demuxer::openFile(std::string inputFileName) {
    std::unique_lock<std::mutex> ul(opLock);
    filename = inputFileName;
    needFlush = true;
    flushFinish = 0;
    while (flushFinish == 0) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        opCv.wait(ul);
    }
    return flushFinish;
}

int Demuxer::openFile() {
    if (avformat_open_input(&fmtCtx, filename.c_str(), nullptr, nullptr) < 0) {
        qWarning("Cannot open input file.");
        cleanUp();
        isEof = true;
        return -1;
    }

    if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
        qWarning("Cannot find any stream in file.");
        cleanUp();
        isEof = true;
        return -1;
    }

    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++) {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1)
            videoStreamIndex = i;

        else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex == -1)
            audioStreamIndex = i;
    }

    if (videoStreamIndex < 0) {
        qWarning("Cannot find video stream in file.\n");
    }

    if (audioStreamIndex < 0) {
        qWarning("Cannot find audio stream in file.\n");
    }

    if (audioStreamIndex < 0 && videoStreamIndex < 0) {
        qWarning("can find any stream\n");
        goto error;
    }

    if (videoStreamIndex >= 0 && initVideoState() < 0) {
        qWarning("error init video state\n");
        goto error;
    }

    if (audioStreamIndex >= 0 && initAudioState() < 0) {
        qWarning("error init audio state\n");
        goto error;
    }

    isEof = false;
    isPause = false;
    return 1;

    error:
    cleanUp();
    isEof = true;
    return -1;
}

void Demuxer::initDemuxer() {
    workerDemuxer = std::thread(&Demuxer::demuxer, this);
}

int Demuxer::seek(int64_t us) {
    std::unique_lock<std::mutex> ul(opLock);
    isSeek = true;
    seekFinish = false;
    targetUs = us;
    while (!seekFinish) {
        opCv.wait(ul);
    }
    return seekRet;
}

void Demuxer::quit() {
    isQuit = true;
}

void Demuxer::pause() {
    isPause = true;
}

void Demuxer::resume() {
    isPause = false;
}

void Demuxer::demuxer() {
    int ret = 0;
    for (;;) {
        if (isQuit) {
            break;
        }

        {
            std::unique_lock<std::mutex> ul(opLock);
            if (isSeek) {
                seekCleanUp();
                if (fmtCtx) {
                    seekRet = av_seek_frame(fmtCtx, -1,
                                            static_cast<int64_t>(targetUs / 1000000.0 * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
                    if (seekRet < 0) {
                        debugErr("av_seek_frame", seekRet);
                    }
                    isPause = false;
                    isEof = false;
                }
                seekFinish = true;
                isSeek = false;
                opCv.notify_all();
                ul.unlock();
                continue;
            }

            if (needFlush) {
                cleanUp();
                flushFinish = openFile();
                needFlush = false;
                opCv.notify_all();
                ul.unlock();
                continue;
            }
        }

        if (isPause || isEof) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (videoFrameQueue.queue.size() >= 100 && audioFrameQueue.queue.size() >= 100) {
            std::unique_lock<std::mutex> ul(moreFrameLock);
            moreFrameCv.wait_for(ul, std::chrono::milliseconds(10));
            continue;
        }

        ret = av_read_frame(fmtCtx, pkt);
        if (ret < 0) {
            isEof = true;
            videoFrameQueue.pushEof();
            audioFrameQueue.pushEof();
            continue;
        }

        if (pkt->stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(videoCodecCtx, pkt);
            if (ret < 0) {
                debugErr("avcodec_send_packet", ret);
                break;
            }
            while ((ret = avcodec_receive_frame(videoCodecCtx, yuvFrame)) >= 0) {
                auto copyFrame = av_frame_alloc();
                av_frame_move_ref(copyFrame, yuvFrame);
                av_frame_unref(yuvFrame);
                videoFrameQueue.push(copyFrame);
            }
            if (ret < 0) {
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
                else {
                    debugErr("avcodec_receive_frame", ret);
                    break;
                }
            }
        } else if (pkt->stream_index == audioStreamIndex) {
            ret = avcodec_send_packet(audioCodecCtx, pkt);
            if (ret < 0) {
                debugErr("avcodec_send_packet", ret);
                break;
            }
            while ((ret = avcodec_receive_frame(audioCodecCtx, sampleFrame)) >= 0) {
                auto copyFrame = av_frame_alloc();
                av_frame_move_ref(copyFrame, sampleFrame);
                av_frame_unref(sampleFrame);
                audioFrameQueue.push(copyFrame);
            }
            if (ret < 0) {
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
                else {
                    debugErr("avcodec_receive_frame", ret);
                    break;
                }
            }
        }
        av_packet_unref(pkt);
    }
    qDebug("demuxer quit\n");
}

Picture Demuxer::getPicture(bool block) {
    Picture res;
    while (true) {
        if (isQuit) {
            break;
        }
        if (videoFrameQueue.queue.size() < 50)
            moreFrameCv.notify_all();
        auto frame = videoFrameQueue.front();
        if (frame) {
            if (videoFrameQueue.isEof(frame)) {
                videoFrameQueue.pop();
                break;
            }
            double pts = static_cast<double>(frame->pts) * av_q2d(videoStream->time_base);
            res = Picture(frame, pts);
            break;
        } else if (!block)
            break;
        else if (block)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return res;
}

void Demuxer::popPicture() {
    videoFrameQueue.pop();
}

Sample Demuxer::getSample(bool block) {
    Sample res;
    while (true) {
        if (isQuit) {
            break;
        }
        if (audioFrameQueue.queue.size() < 50)
            moreFrameCv.notify_all();
        auto frame = audioFrameQueue.front();
        if (frame) {
            if (audioFrameQueue.isEof(frame)) {
                audioFrameQueue.pop();
                break;
            }
            double pts = static_cast<double>(frame->pts) * av_q2d(audioStream->time_base);
            int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                                  (const uint8_t **) (frame->data), frame->nb_samples);

            int out_size = av_samples_get_buffer_size(0, 2,
                                                      len,
                                                      AV_SAMPLE_FMT_S16,
                                                      1);

            res = Sample(audioOutBuf, out_size, pts, frame);
            break;
        } else if (!block)
            break;
        else if (block)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
    return res;
}

void Demuxer::popSample() {
    audioFrameQueue.pop();
}