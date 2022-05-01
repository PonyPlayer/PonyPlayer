//
// Created by ColorsWind on 2022/5/1.
//
#include <demuxer2.h>


template<DemuxDecoder::DecoderType type>
DecoderImpl<type>::DecoderImpl(AVStream *vs, long long int capacity) : stream(vs), frameQueue(capacity) {
    auto videoCodecPara = stream->codecpar;
    if (!(codec = const_cast<AVCodec *>(avcodec_find_decoder(videoCodecPara->codec_id)))) {
        throw std::runtime_error("Cannot find valid video decode codec.");
    }
    if (!(codecCtx = avcodec_alloc_context3(codec))) {
        throw std::runtime_error("Cannot find valid video decode codec context.");
    }
    if (avcodec_parameters_to_context(codecCtx, videoCodecPara) < 0) {
        throw std::runtime_error("Cannot initialize videoCodecCtx.");
    }
    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        throw std::runtime_error("Cannot open codec.");
    }

    frameBuf = av_frame_alloc();
    if constexpr(type == DemuxDecoder::DecoderType::Audio) {
        this->swrCtx = swr_alloc_set_opts(swrCtx, av_get_default_channel_layout(2),
                                          AV_SAMPLE_FMT_S16, 44100,
                                          static_cast<int64_t>(codecCtx->channel_layout), codecCtx->sample_fmt,
                                          codecCtx->sample_rate, 0, nullptr);

        if (!swrCtx || swr_init(swrCtx) < 0) {
            throw std::runtime_error("Cannot initialize swrCtx");
        }
        if (!(audioOutBuf = static_cast<uint8_t *>(av_malloc(2 * MAX_AUDIO_FRAME_SIZE)))) {
            throw std::runtime_error("Cannot alloc audioOutBuf");
        }
        sampleFrameBuf = av_frame_alloc();
    }
}

template<DemuxDecoder::DecoderType type>
DecoderImpl<type>::~DecoderImpl() {
    if (sampleFrameBuf) { av_frame_free(&sampleFrameBuf); }
    if (audioOutBuf) { av_freep(&audioOutBuf); }
    if (swrCtx) { swr_free(&swrCtx); }
    if (frameBuf) { av_frame_free(&frameBuf); }
    if (codecCtx) { avcodec_close(codecCtx); }
    if (codecCtx) { avcodec_free_context(&codecCtx); }
}

template<DemuxDecoder::DecoderType type>
qreal DecoderImpl<type>::duration() {
    return static_cast<double>(stream->duration) * av_q2d(stream->time_base);
}

template<DemuxDecoder::DecoderType type>
bool DecoderImpl<type>::accept(AVPacket *pkt, std::atomic<bool> &interrupt) {
    int ret = avcodec_send_packet(codecCtx, pkt);
    if (ret < 0) {
        qWarning() << "Error avcodec_send_packet:" << ffmpegErrToString(ret);
        return false;
    }
    while(ret >= 0 && !interrupt) {
        ret = avcodec_receive_frame(codecCtx, frameBuf);
        if (ret >= 0) {
            ret = frameQueue.bpush(frameBuf);
            frameBuf = av_frame_alloc();
        } else if (ret == AVERROR(EAGAIN)) {
            return true;
        } else if (ret == ERROR_EOF) {
            frameQueue.bpush(nullptr);
            return false;
        } else {
            frameQueue.bpush(nullptr);
            qWarning() << "Error avcodec_receive_frame:" << ffmpegErrToString(ret);
            return false;
        }
    }
    return false;
}

template<DemuxDecoder::DecoderType type>
void DecoderImpl<type>::flushFFmpegBuffers() {
    avcodec_flush_buffers(codecCtx);
}

template<DemuxDecoder::DecoderType type>
BlockingQueue<AVFrame *> &DecoderImpl<type>::getFrameQueue() {
    return frameQueue;
}

template<DemuxDecoder::DecoderType type>
Picture DecoderImpl<type>::getPicture(bool b) {
    if constexpr(type != DemuxDecoder::DecoderType::Video) { throw std::runtime_error("Unsupported operation."); }
    AVFrame *frame = frameQueue.bfront();
    if (!frame) { return {}; }
    double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
    return {frame, pts};
}

template<DemuxDecoder::DecoderType type>
Sample DecoderImpl<type>::getSample(bool b) {
    if constexpr(type != DemuxDecoder::DecoderType::Audio) { throw std::runtime_error("Unsupported operation."); }
    AVFrame *frame = frameQueue.bfront();
    if (!frame) { return {}; }
    double pts = static_cast<double>(frame->pts) * av_q2d(stream->time_base);
    int len = swr_convert(swrCtx, &audioOutBuf, 2 * MAX_AUDIO_FRAME_SIZE,
                          const_cast<const uint8_t **>(frame->data), frame->nb_samples);

    int out_size = av_samples_get_buffer_size(0, 2,
                                              len,
                                              AV_SAMPLE_FMT_S16,
                                              1);
    return {audioOutBuf, out_size, pts, frame};
}

template<DemuxDecoder::DecoderType type>
bool DecoderImpl<type>::pop(bool b) {
    return frameQueue.bpop();
}


template class DecoderImpl<Audio>;

template class DecoderImpl<Video>;