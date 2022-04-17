//
// Created by kurisu on 2022/4/16.
//

#ifndef FFMPEGCMAKE_FRAME_QUEUE_H
#define FFMPEGCMAKE_FRAME_QUEUE_H

#include <mutex>
#include <condition_variable>

#include <helper.h>
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
INCLUDE_FFMPEG_END

struct Frame {
    AVFrame *frame{};
    int width{};
    int height{};
    double frameRate{};
};

#define MAXQ 60

class FrameQueue {
private:
    Frame queue[MAXQ];
    int rindex = 0;
    int windex = 0;
    int size = 0;
    std::mutex lock;
    std::condition_variable cv;

public:
    FrameQueue() {
        for (int i = 0; i < MAXQ; i++) {
            if (!(queue[i].frame = av_frame_alloc())) {
                printf("oom\n");
                exit(-1);
            }
        }
    }

    ~FrameQueue() {
        for (int i = 0; i < MAXQ; i++) {
            av_frame_free(&queue[i].frame);
        }
    }

    Frame *front() {
        std::unique_lock<std::mutex> ul(lock);
        while (size == 0) {
            cv.wait(ul);
        }
        return &queue[rindex];
    }

    void pop() {
        std::unique_lock<std::mutex> ul(lock);
        --size;
        // av_frame_unref(queue[rindex].frame);
        ++rindex;
        rindex %= MAXQ;
        cv.notify_all();
    }

    Frame *nextWritePos() {
        std::unique_lock<std::mutex> ul(lock);
        while (size >= MAXQ) {
            cv.wait(ul);
        }
        return &queue[windex];
    }

    void push() {
        std::unique_lock<std::mutex> ul(lock);
        while (size >= MAXQ) {
            cv.wait(ul);
        }
        ++windex;
        ++size;
        windex %= MAXQ;
        cv.notify_all();
    }
};

#endif //FFMPEGCMAKE_FRAME_QUEUE_H
