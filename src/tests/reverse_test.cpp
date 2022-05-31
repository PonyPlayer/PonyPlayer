//
// Created by kurisu on 2022/5/11.
//

#include <gtest/gtest.h>
#include <thread>
#include "private/dispatcher.hpp"

void testGetFrame() {
    auto fn = "D:/test_video/dj.mp4";
    ReverseDecodeDispatcher dispatcher(fn, nullptr);
    auto t1 = std::thread([&]() {
        dispatcher.seek(5.0);
        dispatcher.stateResume();
        dispatcher.onWork();
        std::cerr << "finish" << std::endl;
    });
    t1.join();
    do {
        auto vf = dispatcher.getPicture();
        auto af = dispatcher.getSample();
        if (!af.isValid() || !vf.isValid())
            break;
        std::cerr << vf.getPTS() << " " << af.getPTS() << std::endl;
    } while (true);
}

TEST(reverse_test, testGetFrame) {
    testGetFrame();
}