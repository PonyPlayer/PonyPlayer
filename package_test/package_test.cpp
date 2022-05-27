//
// Created by ColorsWind on 2022/05/27.
//
#include <QCoreApplication>
#include <iostream>
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <libavcodec/avcodec.h>
#pragma GCC diagnostic pop
}
int main(int argc, char **argv) {
    QCoreApplication application(argc, argv);
    AVFrame *frame = av_frame_alloc();
    av_frame_free(&frame);
    std::cout << "Hello World" << std::endl;
    return 0;
}
