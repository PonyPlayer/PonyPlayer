//
// Created by ColorsWind on 2022/4/16.
//
#pragma once

#if defined(WIN32) || defined(linux)
#define INCLUDE_FFMPEG_BEGIN \
extern "C" {       \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wold-style-cast\"") \
_Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
_Pragma("GCC diagnostic ignored \"-Wconversion\"")
#elif defined(__APPLE__)
#define INCLUDE_FFMPEG_BEGIN \
extern "C" {       \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wold-style-cast\"") \
_Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
_Pragma("GCC diagnostic ignored \"-Wimplicit-int-conversion\"")
#endif

#define INCLUDE_FFMPEG_END \
_Pragma("GCC diagnostic pop") \
}


extern "C" {
#include <libavutil/error.h>
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
const int ERROR_EOF = AVERROR_EOF;
const int MAX_AUDIO_FRAME_SIZE = 16384;
const int AUDIO_DATA_INFO_QUEUE_INITIAL = 100;
#pragma GCC diagnostic pop


inline unsigned nextPowerOf2(unsigned val) {
    val--;
    val = (val >> 1) | val;
    val = (val >> 2) | val;
    val = (val >> 4) | val;
    val = (val >> 8) | val;
    val = (val >> 16) | val;
    return ++val;
}


#include <QString>
inline QString ffmpegErrToString(int err) {
    char errBuf[512];
    av_strerror(err, errBuf, sizeof(errBuf));
    return errBuf;
}

