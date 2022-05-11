//
// Created by kurisu on 2022/5/11.
//

#include <gtest/gtest.h>
#include <thread>
#include "dispatcher.hpp"

void testGetFrame() {
    auto fn = "D:/test_video/dj.mp4";
    ReverseDecodeDispatcher dispatcher(fn, nullptr);
    auto thread = std::thread([&]() {
        std::cerr << dispatcher.getVideoLength() << std::endl;
        dispatcher.seek(5.0);
        dispatcher.stateResume();
        dispatcher.onWork();
    });
    thread.join();
    std::cerr << std::endl;
    VideoFrame vf;
    do {
        vf = dispatcher.getPicture(false, true);
        std::cerr << vf.getPTS() << std::endl;
        dispatcher.popPicture(false);
    } while (vf.isValid());
    //std::this_thread::sleep_for(std::chrono::seconds(5));
}

TEST(reverse_test, testGetFrame) {
    testGetFrame();
}