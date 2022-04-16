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

class PacketQueue {
private:
    std::list<AVPacket> queue;
    std::mutex lock;
    std::condition_variable cv;

public:
    void push(AVPacket* pkt) {
        std::unique_lock<std::mutex> ul(lock);
        queue.push_back(*pkt);
        cv.notify_all();
    }

    AVPacket pop() {
        std::unique_lock<std::mutex> ul(lock);
        while (queue.empty()) {
            cv.wait(ul);
        }
        auto res = queue.front();
        queue.pop_front();
        return res;
    }
};

#endif //FFMPEGCMAKE_PACKETQUEUE_H
