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
            frame(frame_), pts(pts_), valid(true) {}

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

    [[nodiscard]] uint8_t *getY() const {
        return frame->data[0];
    }

    [[nodiscard]] uint8_t *getU() const {
        return frame->data[1];
    }

    [[nodiscard]] uint8_t *getV() const {
        return frame->data[2];
    }

    [[nodiscard]] int getLineSize() const {
        return frame->linesize[0];
    }

    [[nodiscard]] int getWidth() const {
        return frame->width;
    }

    [[nodiscard]] int getHeight() const {
        return frame->height;
    }

    /**
     * 如果picture不包含有效数据，不要调用free
     */
    void free() {
        if (frame) av_frame_free(&frame);
    }
};

struct FrameQueue {
    std::queue<AVFrame *> queue;
    int maxSize{};
    bool isQuit{};
    std::mutex lock;
    std::condition_variable cv;

    explicit FrameQueue(int maxSize_ = 30) : maxSize(maxSize_) {}

    ~FrameQueue() {
        while (!queue.empty()) {
            av_frame_free(&queue.front());
            queue.pop();
        }
    }

    AVFrame *front(bool block) {
        std::unique_lock<std::mutex> ul(lock);
        while (true) {
            if (isQuit)
                return nullptr;
            if (!queue.empty())
                break;
            else if (block)
                cv.wait_for(ul, std::chrono::milliseconds(10));
            else
                return nullptr;
        }
        return queue.front();
    }

    void pop() {
        std::unique_lock<std::mutex> ul(lock);
        if (queue.empty())
            return;
        queue.pop();
        cv.notify_all();
    }

    void push(AVFrame *frame) {
        std::unique_lock<std::mutex> ul(lock);
        while (true) {
            if (isQuit)
                return;
            if (static_cast<int>(queue.size()) >= maxSize) {
                cv.wait_for(ul, std::chrono::milliseconds(10));
            } else {
                queue.push(frame);
                break;
            }
        }
        cv.notify_all();
    }
};

#endif //FFMPEGCMAKE_FRAME_QUEUE_H
