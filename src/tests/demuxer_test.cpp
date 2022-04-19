//
// Created by kurisu on 2022/4/19.
//
#include <gtest/gtest.h>
#include "demuxer.h"

TEST(frame_test, save_frame) {
    test_saveFrame("D:/test_video/weird.mov", 10);
}

TEST(quit_test, test_quit) {
    test_quit("D:/test_video/1.mp4");
}