//
// Created by ColorsWind on 2022/5/7.
//
#pragma once

#include "helper.hpp"
#include "ponyplayer.h"
#include <queue>
#include <mutex>
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
INCLUDE_FFMPEG_END

typedef std::function<void(AVFrame*)> FrameFreeFunc;

class VideoFrame {
private:
    AVFrame *m_frame;
    double m_pts;
    const FrameFreeFunc *m_freeFunc;
    bool m_isValid;
public:
    VideoFrame(AVFrame *frame, double pts, const FrameFreeFunc *freeFunc, bool isValid) :
        m_frame(frame), m_pts(pts), m_freeFunc(freeFunc), m_isValid(isValid) {}

    VideoFrame(AVFrame *frame, double pts, const FrameFreeFunc *freeFunc) :
        m_frame(frame), m_pts(pts), m_freeFunc(freeFunc), m_isValid(true) {}

    VideoFrame() : m_frame(nullptr), m_pts(std::numeric_limits<double>::quiet_NaN()), m_isValid(false) {}

    /**
     * @return 图像数据是否有效
     */
    [[nodiscard]] bool isValid() const {
        return m_isValid;
    }


    [[nodiscard]] double getPTS() const {
        return m_pts;
    }

    [[nodiscard]] std::byte *getY() const {
        return reinterpret_cast<std::byte*>(m_frame->data[0]);
    }

    [[nodiscard]] std::byte *getU() const {
        return reinterpret_cast<std::byte*>(m_frame->data[1]);
    }

    [[nodiscard]] std::byte *getV() const {
        return reinterpret_cast<std::byte*>(m_frame->data[2]);
    }

    [[nodiscard]] int getLineSize() const {
        return m_frame->linesize[0];
    }

    [[nodiscard]] int getWidth() const {
        return m_frame->width;
    }

    [[nodiscard]] int getHeight() const {
        return m_frame->height;
    }

    [[nodiscard]] bool isSameSize(const VideoFrame &frame) const {
        return (this->isValid()
            && frame.isValid()
            && getWidth() == frame.getWidth()
            && getHeight() == frame.getHeight()
            && getLineSize() == frame.getLineSize())
            || (!this->isValid() && !frame.isValid());
    }

    [[nodiscard]] bool isPlaceholderFrame() const {
        return this->m_pts < 0;
    }

    bool operator==(const VideoFrame &frame) const {
        return this->m_frame == frame.m_frame;
    }

    bool operator!=(const VideoFrame &frame) const {
        return !this->operator==(frame);
    }

    /**
     * 将当前 VideoFrame 标记为 Invalid
     * e.g 当前 VideoFrame 内部数据交由令一个 VideoFrame 管理
     */
    void makeInvalid() {
        m_isValid = false;
    }

    /**
     * 如果picture不包含有效数据，不要调用free
     */
    void free() {
        // NOTE: CANNOT guarantee that it will NOT be double free under multithreading codition
        if (!m_freeFunc) {
            m_frame = nullptr;
            m_isValid = false;
        } else if (m_frame) {
            (*m_freeFunc)(m_frame);
            m_frame = nullptr;
            m_isValid = false;
        }
    }
};

class AudioFrame {
private:
    std::byte *m_data;
    int m_len;
    double m_pts;
public:
    AudioFrame(): m_data(nullptr), m_len(0), m_pts(std::numeric_limits<double>::quiet_NaN())  {}

    AudioFrame(std::byte *data, int len, double pts) : m_data(data), m_len(len), m_pts(pts) {}

    bool isValid() {
        return m_data;
    }

    [[nodiscard]] std::byte *getSampleData() const{
        return m_data;
    }

    [[nodiscard]] int getDataLen() const {
        return m_len;
    }

    [[nodiscard]] double getPTS() const {
        return m_pts;
    }
};


class LifeCycleManager {
    // memory_order_seq_cst
protected:
    std::atomic<int> refCount = 0;
    bool autoDelete = false;

    std::mutex mutex;
    std::queue<AVFrame *> freeQueue;
public:
    const FrameFreeFunc freeFunc = [this](AVFrame *frame) { this->freeFrame(frame); };

    PONY_THREAD_SAFE void pop() {
        refCount += 1;
    }

    PONY_THREAD_SAFE void freeFrame(AVFrame *frame) {
        int ret = --refCount;
        av_frame_free(&frame);
        if (ret == 0 && autoDelete) { delete this; }
    }

    PONY_THREAD_SAFE void deleteLater() {
        int expect = 0;
        if (refCount.compare_exchange_strong(expect, -1)) {
            delete this;
        } else {
            autoDelete = true;
        }
    }

    void freeLater(AVFrame *frame) {
        std::unique_lock lock(mutex);
        freeQueue.push(frame);
    }

    ~LifeCycleManager() {
        std::unique_lock lock(mutex);
        while(!freeQueue.empty()) {
            av_frame_free(&freeQueue.front());
            freeQueue.pop();
        }
    }
};


