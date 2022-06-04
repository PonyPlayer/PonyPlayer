//
// Created by ColorsWind on 2022/4/16.
//
#pragma once



extern "C" {
#include <libavutil/error.h>
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
const int ERROR_EOF = AVERROR_EOF;
const int MAX_AUDIO_FRAME_SIZE = 16384;
const int AUDIO_DATA_INFO_QUEUE_INITIAL = 100;
#pragma GCC diagnostic pop


#include <QString>
inline QString ffmpegErrToString(int err) {
    char errBuf[512];
    av_strerror(err, errBuf, sizeof(errBuf));
    return errBuf;
}

