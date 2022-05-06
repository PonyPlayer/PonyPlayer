//
// Created by ColorsWind on 2022/4/30.
//
#include "demuxer2.h"

DemuxDispatcherBase::DemuxDispatcherBase(const std::string &fn, QObject *parent) : QObject(parent), filename(fn) {
    if (avformat_open_input(&fmtCtx, fn.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Cannot open input file.");
    }
    if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
        throw std::runtime_error("Cannot find any stream in file.");
    }
}

DemuxDispatcherBase::~DemuxDispatcherBase() {
    if (fmtCtx) { avformat_close_input(&fmtCtx); }
}


DecodeDispatcher::DecodeDispatcher(const std::string &fn, QObject *parent) : DemuxDispatcherBase(fn, parent) {
    packet = av_packet_alloc();
    for (unsigned int i = 0; i < fmtCtx->nb_streams; ++i) {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex.emplace_back(i);
        } else if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex.emplace_back(i);
        }
    }
    if (videoStreamIndex.empty()) { throw std::runtime_error("Cannot find video stream."); }
    if (audioStreamIndex.empty()) { throw std::runtime_error("Cannot find audio stream."); }
    decoders.resize(fmtCtx->nb_streams);
    // WARNING: the capacity of queue must >= 2 * the maximum number of frame of packet
    videoQueue = new TwinsBlockQueue<AVFrame *>("VideoQueue", 16);
    audioQueue = videoQueue->twins("AudioQueue", 16);
    decoders[videoStreamIndex.front()] = new DecoderImpl<Video>(fmtCtx->streams[videoStreamIndex.front()], videoQueue);
    decoders[audioStreamIndex.front()] = new DecoderImpl<Audio>(fmtCtx->streams[audioStreamIndex.front()], audioQueue);


    interrupt = false; // happens before
}

DecodeDispatcher::~DecodeDispatcher() {
    qDebug() << "Destroy decode dispatcher " << filename.c_str();
    for(auto && decoder : decoders) { delete decoder; }
    decoders.clear();
    if(packet) { av_packet_free(&packet); }
}

void DecodeDispatcher::flush() {
    videoQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
    audioQueue->clear([](AVFrame *frame){ av_frame_free(&frame);});
}

void DecodeDispatcher::seek(int64_t us) {
    // case 1: currently decoding, interrupt
    // case 2: not decoding, seek
    interrupt = true;
    for (const auto & decoder : decoders) {
        // interrupt
        if (decoder) decoder->flushFFmpegBuffers();
    }
    qDebug() << "a Seek:" << us;
    int ret = av_seek_frame(fmtCtx, -1, static_cast<int64_t>(static_cast<double>(us) / 1000000.0 * AV_TIME_BASE), AVSEEK_FLAG_BACKWARD);
    if (ret != 0) { qWarning() << "Error av_seek_frame:" << ffmpegErrToString(ret); }

}

void DecodeDispatcher::onWork() {
    interrupt = false;
    videoQueue->open();
    while(!interrupt) {
        int ret = av_read_frame(fmtCtx, packet);
        if (ret == 0) {
            auto *decoder = decoders[static_cast<unsigned long>(packet->stream_index)];
            if (decoder) {
                ret = decoder->accept(packet, interrupt);
                av_packet_unref(packet);
                if (!ret) {
                    videoQueue->push(nullptr);
                    audioQueue->push(nullptr);
                    // no more packet
                    break;
                }
            } else { av_packet_unref(packet); }
        } else {
            if (ret == ERROR_EOF) {
                videoQueue->push(nullptr);
                audioQueue->push(nullptr);
            }  else {
                qWarning() << "Error av_read_frame:" << ffmpegErrToString(ret);
            }
            av_packet_unref(packet);
            break;
        }
        QCoreApplication::processEvents();
    }
    interrupt = true;
}
