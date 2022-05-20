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
        if (m_videoFrame) { m_videoFrame->unref(); }
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

