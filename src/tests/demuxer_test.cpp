//
// Created by kurisu on 2022/4/19.
//
#include <gtest/gtest.h>
#include "demuxer.h"

TEST(frame_test, save_frame) {
    test_saveFrame("D:/test_video/weird.mov", 100);
}

TEST(quit_test, test_quit) {
    test_quit("D:/test_video/1.mp4");
}

TEST(pause_test, test_pause) {
    test_pause("D:/test_video/1.mp4", true);
}

TEST(audio_test, test_audio) {
    test_audio("D:/test_video/1.mp4");
}

TEST(pcm_test, test_savePCM) {
    test_savePCM("D:/test_video/1.mp4");
}

TEST(eof_test, test_eof) {
    test_eof("D:/test_video/1.mp4");
}

TEST(seek_test, test_seek) {
    test_seek("D:/test_video/1.mp4");
}