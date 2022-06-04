//
// Created by kurisu on 2022/6/4.
//
#include <gtest/gtest.h>
#include "frame.hpp"

TEST(frame_test, frame_free) {
    auto frame = av_frame_alloc();
    {
        VideoFrameRef frameRef(frame, true, 0.0);
    }
    EXPECT_EQ(VideoFrame::totalCount, 0);
}

TEST(frame_test, frame_copy_assign) {
    auto frame = av_frame_alloc();
    {
        VideoFrameRef frameRef1;
        {
            VideoFrameRef frameRef(frame, true, 0.0);
            frameRef1 = frameRef;
            EXPECT_EQ(VideoFrame::totalCount, 1);
        }
        EXPECT_EQ(VideoFrame::totalCount, 1);
    }
    EXPECT_EQ(VideoFrame::totalCount, 0);
}

TEST(frame_test, frame_copy_construct) {
    auto frame = av_frame_alloc();
    {
        VideoFrameRef frameRef(frame, true, 0.0);
        {
            VideoFrameRef frameRef1 = frameRef;
            EXPECT_EQ(VideoFrame::totalCount, 1);
        }
        EXPECT_EQ(VideoFrame::totalCount, 1);
    }
    EXPECT_EQ(VideoFrame::totalCount, 0);
}

TEST(frame_test, frame_move_construct) {
    auto frame = av_frame_alloc();
    {
        VideoFrameRef frameRef = VideoFrameRef(frame, true, 0.0);
        EXPECT_EQ(VideoFrame::totalCount, 1);
    }
    EXPECT_EQ(VideoFrame::totalCount, 0);
}

TEST(frame_test, frame_move_assign) {
    auto frame = av_frame_alloc();
    {
        VideoFrameRef frameRef1;
        {
            VideoFrameRef frameRef(frame, true, 0.0);
            frameRef1 = std::move(frameRef);
            EXPECT_EQ(VideoFrame::totalCount, 1);
        }
        EXPECT_EQ(VideoFrame::totalCount, 1);
    }
    EXPECT_EQ(VideoFrame::totalCount, 0);
}