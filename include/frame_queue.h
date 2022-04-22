//
// Created by kurisu on 2022/4/16.
//

#ifndef FFMPEGCMAKE_FRAME_QUEUE_H
#define FFMPEGCMAKE_FRAME_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>

#include <helper.h>

INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
INCLUDE_FFMPEG_END

struct Picture {
    AVFrame *frame{};
    double pts{};
    bool valid{};

    Picture(AVFrame *frame_, double pts_) :
            frame(av_frame_alloc()), pts(pts_), valid(true) {
        av_frame_move_ref(frame, frame_);
        av_frame_unref(frame_);
    }

    Picture() = default;

    /**
     * @return 图像数据是否有效
     */
    bool isValid() {
        return valid;
    }

    double getPTS() {
        return pts;
    }

    uint8_t *getY() {
        return frame->data[0];
    }

    uint8_t *getU() {
        return frame->data[1];
    }

    uint8_t *getV() {
        return frame->data[2];
    }

    int getLineSize() {
        return frame->linesize[0];
    }

    int getWidth() {
        return frame->width;
    }

    int getHeight() {
        return frame->height;
    }

    /**
     * 如果picture不包含有效数据，不要调用free
     */
    void free() {
        if (frame) av_frame_free(&frame);
    }
};

const int MAXQ = 30;

struct Frame {
    AVFrame *frame{};
};

struct FrameQueue {
    Frame queue[MAXQ];
    int rindex = 0;
    int windex = 0;
    int size = 0;
    bool isQuit{};
    std::mutex lock;
    std::condition_variable cv;

    FrameQueue() {
        for (int i = 0; i < MAXQ; i++) {
            queue[i].frame = av_frame_alloc();
            if (queue[i].frame == nullptr) {
                printf("at %d oom, size = %d\n", i, size);
                exit(-1);
            }
        }
    }

    ~FrameQueue() {
        for (int i = 0; i < MAXQ; i++) {
            av_frame_free(&queue[i].frame);
        }
    }

    Frame *front(bool block) {
        std::unique_lock<std::mutex> ul(lock);
        while (true) {
            if (isQuit)
                return nullptr;
            if (size != 0)
                break;
            else if (block)
                cv.wait_for(ul, std::chrono::milliseconds(10));
            else
                return nullptr;
        }
        return &queue[rindex];
    }

    void pop() {
        std::unique_lock<std::mutex> ul(lock);
        if (size == 0)
            return;
        --size;
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
        ++windex;
        ++size;
        windex %= MAXQ;
        cv.notify_all();
    }
};

#endif //FFMPEGCMAKE_FRAME_QUEUE_H
