//
// Created by ColorsWind on 2022/5/20.
//
#pragma once
#include <utility>
#include "frame.hpp"

template<typename T>
class UpdateValue {
protected:
    T m_value;
    bool m_update;
public:

    UpdateValue() : m_value(), m_update(true) {}
    explicit UpdateValue(T value) : m_value(std::move(value)), m_update(true) {}


    operator const T&() const { // NOLINT(google-explicit-constructor)
        return m_value;
    }

    virtual const T& getUpdate() {
        m_update = false;
        return m_value;
    }

    virtual UpdateValue& operator=(const T &v) {
        this->m_value = v;
        m_update = true;
        return *this;
    }

    void updateBy(UpdateValue& updateValue) {
        this->m_update |= updateValue.m_update;
        this->m_value = updateValue.getUpdate();
    }

    [[nodiscard]] bool isUpdate() const {
        return m_update;
    }
};

class UpdateValueVideoFrameRef : public UpdateValue<VideoFrameRef> {
private:
    bool m_updateSize;
public:
    UpdateValueVideoFrameRef() : UpdateValue(VideoFrameRef{}), m_updateSize(true) {}

    explicit UpdateValueVideoFrameRef(const VideoFrameRef &value) : UpdateValue(value), m_updateSize(true) {}



    UpdateValueVideoFrameRef& operator=(const VideoFrameRef &videoFrame) override {
        if (videoFrame == this->m_value) { return *this; }
        if (m_value.isSameSize(videoFrame)) { this->m_updateSize = true; }
        this->m_value = videoFrame;
        m_update = true;
        return *this;
    }

    const VideoFrameRef& getUpdate() override {
        m_update = false;
        m_updateSize = false;
        return m_value;
    }

    void updateBy(UpdateValueVideoFrameRef& updateValue) {
        this->m_update |= updateValue.m_update;
        this->m_updateSize |= updateValue.m_updateSize;
        this->m_value = updateValue.getUpdate();
    }

    bool isUpdateSize() const {
        return m_updateSize;
    }
};

