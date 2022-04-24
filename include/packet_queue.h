//
// Created by kurisu on 2022/4/15.
//

#ifndef FFMPEGCMAKE_PACKETQUEUE_H
#define FFMPEGCMAKE_PACKETQUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>
#include <helper.h>

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
INCLUDE_FFMPEG_END

struct PacketQueue {
    std::list<AVPacket> queue;
    std::mutex lock;
    std::condition_variable cv;
    bool isQuit{};

    void flush() {
        std::unique_lock<std::mutex> ul(lock);
        while (!queue.empty()) {
            av_packet_unref(&queue.front());
            queue.pop_front();
        }
    }

    void push(AVPacket *pkt) {
        std::unique_lock<std::mutex> ul(lock);
        queue.push_back(*pkt);
        cv.notify_all();
    }

    int pop(AVPacket *receiver) {
        std::unique_lock<std::mutex> ul(lock);
        if (isQuit || queue.empty()) {
            return -1;
        }
        *receiver = queue.front();
        queue.pop_front();
        return 1;
    }
};

#endif //FFMPEGCMAKE_PACKETQUEUE_H
