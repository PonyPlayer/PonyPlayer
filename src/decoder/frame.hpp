//
// Created by ColorsWind on 2022/5/7.
//
#pragma once

#include "helper.hpp"
#include "ponyplayer.h"
#include <mutex>
#include <functional>
#include <queue>
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
INCLUDE_FFMPEG_END

typedef std::function<void(AVFrame*)> FrameFreeFunc;

class VideoFrame {
    AVFrame *m_frame;
    std::atomic<int> m_refCount;
    const double m_pts;
    const bool m_isValid;
    friend class VideoFrameRef;
public:

    VideoFrame(AVFrame *frame, double pts, const bool isValid)
            : m_frame(frame), m_refCount(1), m_pts(pts), m_isValid(isValid) {}

    ~VideoFrame() {
        if (m_frame) {
            av_frame_free(&m_frame);
        }
    }

    void unref() {
        if (--m_refCount == 0) {
            delete this;
        }
    }
};

class VideoFrameRef {
private:
    VideoFrame *m_videoFrame;
public:
    VideoFrameRef(AVFrame *frame, bool isValid, double pts) {
        m_videoFrame = new VideoFrame(frame, pts, isValid);
    }

    VideoFrameRef() : VideoFrameRef(nullptr, false, std::numeric_limits<double>::quiet_NaN()) {}

    VideoFrameRef(VideoFrameRef &&rhs) noexcept : m_videoFrame(rhs.m_videoFrame) {
        rhs.m_videoFrame = nullptr;
    }

    VideoFrameRef(const VideoFrameRef &rhs) : m_videoFrame(rhs.m_videoFrame) {
        ++rhs.m_videoFrame->m_refCount;
    }

    VideoFrameRef& operator=(VideoFrameRef &&rhs) noexcept {
        this->m_videoFrame = rhs.m_videoFrame;
        rhs.m_videoFrame = nullptr;
        return *this;
    }

    VideoFrameRef& operator=(const VideoFrameRef &rhs) noexcept {
        if (rhs != *this) {
            this->m_videoFrame = rhs.m_videoFrame;
            ++m_videoFrame->m_refCount;
        }
        return *this;
    }



    ~VideoFrameRef() {
        m_videoFrame->unref();
    }

    bool operator==(const VideoFrameRef &frame) const {
        return this->m_videoFrame == frame.m_videoFrame;
    }

    bool operator!=(const VideoFrameRef &frame) const {
        return !this->operator==(frame);
    }

    /**
     * @return 图像数据是否有效
     */
    [[nodiscard]] bool isValid() const {
        return m_videoFrame && m_videoFrame->m_isValid;
    }


    [[nodiscard]] double getPTS() const {
        return m_videoFrame->m_pts;
    }

    [[nodiscard]] std::byte *getY() const {
        return reinterpret_cast<std::byte*>(m_videoFrame->m_frame->data[0]);
    }

    [[nodiscard]] std::byte *getU() const {
        return reinterpret_cast<std::byte*>(m_videoFrame->m_frame->data[1]);
    }

    [[nodiscard]] std::byte *getV() const {
        return reinterpret_cast<std::byte*>(m_videoFrame->m_frame->data[2]);
    }

    [[nodiscard]] int getLineSize() const {
        return m_videoFrame->m_frame->linesize[0];
    }

    [[nodiscard]] int getWidth() const {
        return m_videoFrame->m_frame->width;
    }

    [[nodiscard]] int getHeight() const {
        return m_videoFrame->m_frame->height;
    }

    [[nodiscard]] bool isSameSize(const VideoFrameRef &frame) const {
        return (this->isValid()
                && frame.isValid()
                && this->getWidth() == frame.getWidth()
                && this->getHeight() == frame.getHeight()
                && this->getLineSize() == frame.getLineSize())
               || (!this->isValid() && !frame.isValid());
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


/**
 * @brief 生命周期管理器, 管理脱离 Queue 的 Frame 的生命周期
 * 使用指南: \n
 * 1. 在构造函数中使用new关键字创建 LifeCycleManager \n
 * 2. 在析构函数调用 deleteLater  \n
 * 3. 每次将 AVFrame 所有权交给 LifeCycleManager 时调用 pop \n
 * 4. 使用 LifeCycleManager 的 freeFunc 作为 AVFrame 的清理函数 \n
 * 5. 如果希望 AVFrame 的生命周期与 LifeCycleManager 一致, 调用 freeLater 且不需要调用 pop
 */
class LifeCycleManager {
    // memory_order_seq_cst
protected:
    std::atomic<int> refCount = 0;
    bool autoDelete = false;

    std::mutex mutex;
    std::queue<AVFrame *> freeQueue;
public:
    /**
     * 释放 LifeCycleManager 所有的 Frame
     */
    const FrameFreeFunc freeFunc = [this](AVFrame *frame) { this->freeFrame(frame); };

    /**
     * 直接释放非 LifeCycleManager 所有的 Frame
     */
    const FrameFreeFunc freeDirectFunc = [this](AVFrame *frame) { refCount.load(std::memory_order::seq_cst);
        av_frame_free(&frame); };

    /**
     * Frame 的所有权从 Queue 交给 LifeCycleManager, 引用计数 +1
     */
    PONY_THREAD_SAFE void pop() {
        int ret = refCount++;
        if (ret < 0) {
            throw std::runtime_error("Negative refCount, Potential double free at pop.");
        }
    }

    /**
     * 释放 LifeCycleManager 所有的 Frame, 且引用计数 -1
     * @param frame
     */
    PONY_THREAD_SAFE void freeFrame(AVFrame *frame) {
        int ret = --refCount;
        if (ret < 0) {
            throw std::runtime_error("Negative refCount, Potential double free.");
        }
        av_frame_free(&frame);
        int expect = 0;
        if (refCount.compare_exchange_strong(expect, -1) && autoDelete) { delete this; }
    }

    /**
     * 保证不再会有新的 pop 请求, 当 LifeCycleManager 所有的 Frame 被释放后, LifeCycleManager 会自动删除.
     */
    PONY_THREAD_SAFE void deleteLater() {
        int expect = 0;
        if (refCount.compare_exchange_strong(expect, -1)) {
            delete this;
        } else {
            autoDelete = true;
        }
    }

    /**
     * 将非 LifeCycleManager 所有的 Frame 移入清理队列, 当 LifeCycleManager 释放时, 这些 Frame 会被释放.
     * @param frame
     */
    PONY_THREAD_SAFE void freeLater(AVFrame *frame) {
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


