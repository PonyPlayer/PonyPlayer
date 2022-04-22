//
// Created by kurisu on 2022/4/19.
//

#include "demuxer.h"

#include <QImage>

void test_saveFrame(std::string filename, int n) {

}

void test_quit(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int cnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto picture = demuxer.getPicture(false);
            if (picture.isValid()) {
                ++cnt;
                printf("%d %f\n", cnt, picture.getPTS());
                picture.free();
            } else if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    quit = true;
    demuxer.quit();
    worker.join();
}

void test_pause(std::string filename, bool halfQuit) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    bool quit = false;
    int cnt = 0;
    auto worker = std::thread([&]() {
        for (;;) {
            auto picture = demuxer.getPicture(true);
            if (picture.isValid()) {
                printf("%d %f\n", ++cnt, picture.getPTS());
                picture.free();
            } else if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    demuxer.pause();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    /**
     * halfQuit测试在暂停过程中能否退出
     */
    if (halfQuit) {
        demuxer.quit();
        quit = true;
    }
    else {
        demuxer.resume();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        demuxer.quit();
        quit = true;
    }
    worker.join();
}