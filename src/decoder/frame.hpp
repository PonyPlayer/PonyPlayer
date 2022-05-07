//
// Created by ColorsWind on 2022/5/7.
//

#ifndef PONYPLAYER_FRAME_HPP
#define PONYPLAYER_FRAME_HPP
#include "helper.hpp"
INCLUDE_FFMPEG_BEGIN
#include <libavformat/avformat.h>
INCLUDE_FFMPEG_END

typedef std::function<void(AVFrame**)> FrameFreeFunc;

class VideoFrame {
private:
    AVFrame *m_frame;
    double m_pts;
    FrameFreeFunc m_freeFunc;
public:
    VideoFrame(AVFrame *frame, double pts, FrameFreeFunc freeFunc) : m_frame(frame), m_pts(pts), m_freeFunc(std::move(freeFunc)) {}

    VideoFrame() : m_frame(nullptr), m_pts(std::numeric_limits<double>::quiet_NaN()) {}

    /**
     * @return 图像数据是否有效
     */
    [[nodiscard]] inline bool isValid() const {
        return m_frame;
    }

    [[nodiscard]] inline double getPTS() const {
        return m_pts;
    }

    [[nodiscard]] inline std::byte *getY() const {
        return reinterpret_cast<std::byte*>(m_frame->data[0]);
    }

    [[nodiscard]] inline std::byte *getU() const {
        return reinterpret_cast<std::byte*>(m_frame->data[1]);
    }

    [[nodiscard]] inline std::byte *getV() const {
        return reinterpret_cast<std::byte*>(m_frame->data[2]);
    }

    [[nodiscard]] inline int getLineSize() const {
        return m_frame->linesize[0];
    }

    [[nodiscard]] inline int getWidth() const {
        return m_frame->width;
    }

    [[nodiscard]] inline int getHeight() const {
        return m_frame->height;
    }

    [[nodiscard]] inline bool isFree() const {
        return !m_frame;
    }

    bool operator==(const VideoFrame &frame) const {
        return this->m_frame == frame.m_frame;
    }

    bool operator!=(const VideoFrame &frame) const {
        return !this->operator==(frame);
    }

    /**
     * 如果picture不包含有效数据，不要调用free
     */
    void free() {
        if (m_frame) { m_freeFunc(&m_frame); }
    }
};

class AudioFrame {
private:
    AVFrame *m_frame;
    std::byte *m_data;
    int m_len;
    double m_pts;
public:
    AudioFrame(): m_frame(nullptr), m_data(nullptr), m_len(0), m_pts(std::numeric_limits<double>::quiet_NaN())  {}

    AudioFrame(std::byte *data, int len, double pts, AVFrame *frame) : m_frame(frame), m_data(data), m_len(len), m_pts(pts) {}

    bool isValid() {
        return m_frame;
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


#endif //PONYPLAYER_FRAME_HPP
