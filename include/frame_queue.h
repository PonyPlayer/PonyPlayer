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
    [[nodiscard]] inline bool isValid() {
        return valid;
    }

    [[nodiscard]] inline double getPTS() {
        return pts;
    }

    [[nodiscard]] inline uint8_t *getY() const {
        return frame->data[0];
    }

    [[nodiscard]] inline uint8_t *getU() const {
        return frame->data[1];
    }

    [[nodiscard]] inline uint8_t *getV() const {
        return frame->data[2];
    }

    [[nodiscard]] inline int getLineSize() const {
        return frame->linesize[0];
    }

    [[nodiscard]] inline int getWidth() const {
        return frame->width;
    }

    [[nodiscard]] inline int getHeight() const {
        return frame->height;
    }

    [[nodiscard]] inline bool isFree() const {
        return !frame;
    }



    /**
     * 如果picture不包含有效数据，不要调用free
     */
    void free() {
        if (frame) av_frame_free(&frame);
    }
};

struct Sample {
    uint8_t *data{};
    int len{};
    double pts{};
    bool valid{};
    AVFrame *frame{};

    Sample()  {
        data = nullptr;
        len = 0;
        pts = std::numeric_limits<double>::quiet_NaN();
        valid = false;
        frame = nullptr;
    }

    Sample(uint8_t *data_, int len_, double pts_, AVFrame *frame_) :
            data(data_), len(len_), pts(pts_), valid(true), frame(frame_) {}

    bool isValid() {
        return valid;
    }

    uint8_t *getSampleData() {
        return data;
    }

    int getDataLen() {
        return len;
    }

    double getPTS() {
        return pts;
    }

    void free() {
        if (frame) av_frame_free(&frame);
    }
};

struct FrameQueue {
    std::queue<AVFrame *> queue;
    std::mutex lock;
    AVFrame *eofFrame;

    FrameQueue() : eofFrame(av_frame_alloc()) {}

    ~FrameQueue() {
        flush();
    }

    bool isEof(AVFrame *frame) {
        return frame == eofFrame;
    }

    void pushEof() {
        std::unique_lock<std::mutex> ul(lock);
        queue.push(eofFrame);
    }

    void flush() {
        std::unique_lock<std::mutex> ul(lock);
        while (!queue.empty()) {
            auto frame = queue.front();
            if (frame != eofFrame)
                av_frame_free(&frame);
            queue.pop();
        }
    }

    AVFrame *front() {
        std::unique_lock<std::mutex> ul(lock);
        if (queue.empty())
            return nullptr;
        return queue.front();
    }

    void pop() {
        std::unique_lock<std::mutex> ul(lock);
        if (queue.empty())
            return;
        queue.pop();
    }

    void push(AVFrame *frame) {
        std::unique_lock<std::mutex> ul(lock);
        queue.push(frame);
    }
};


#endif //FFMPEGCMAKE_FRAME_QUEUE_H
