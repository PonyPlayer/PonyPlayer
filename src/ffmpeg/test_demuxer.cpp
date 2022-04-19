//
// Created by kurisu on 2022/4/19.
//

#include "demuxer.h"

#include <QImage>

void test_saveFrame(std::string filename, int n) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int cnt = 0;
    auto worker = std::thread([&]() {
        for (;;) {
            auto frame = demuxer.videoFrameQueueFront();
            if (frame) {
                if (cnt < n) {
                    auto rgb = demuxer.toRGB24(frame);
                    QImage img(rgb.frame->data[0], rgb.width,
                               rgb.height, rgb.frame->linesize[0],
                               QImage::Format_RGB888);
                    char buf[64]{};
                    sprintf(buf, "D:/test_video/frame%d.jpg", cnt);
                    img.save(QString(buf), "JPG");
                    ++cnt;
                    demuxer.videoFrameQueuePop();
                } else {
                    demuxer.quit();
                    break;
                }
            }
        }
    });
    worker.join();
}

int test_quit(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int cnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto frame = demuxer.videoFrameQueueFront();
            if (frame) {
                ++cnt;
                printf("%d\n", cnt);
                demuxer.videoFrameQueuePop();
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