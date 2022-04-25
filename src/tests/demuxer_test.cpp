//
// Created by kurisu on 2022/4/19.
//
#include <gtest/gtest.h>
#include "demuxer.h"

TEST(quit_test, test_quit) {
    test_quit("D:/test_video/1.mp4");
}

TEST(pause_test, test_pause) {
    test_pause("D:/test_video/1.mp4", true);
}

TEST(audio_test, test_audio) {
    test_audio("D:/test_video/1.mp4");
}

TEST(seek_test, test_seek) {
    test_seek("D:/test_video/weird.mov");
}