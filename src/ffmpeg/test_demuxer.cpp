//
// Created by kurisu on 2022/4/19.
//

#include "demuxer.h"

#include <QImage>

void saveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[32];
    int y;

    // 打开文件
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");
    if (pFile == nullptr)
        return;

    // 写入文件头
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // 写入像素数据
    for (y = 0; y < height; y++)
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);

    printf("write success\n");
    // 关闭文件
    fclose(pFile);
}

void test_saveFrame(std::string filename)
{
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int cnt = 0;
    for (;;){
        auto frame = demuxer.videoFrameQueueFront();
        if (frame) {
            if (cnt < 5) {
                auto rgb = demuxer.toRGB24(frame);
                QImage img(rgb.frame->data[0], rgb.width,
                           rgb.height, rgb.frame->linesize[0],
                           QImage::Format_RGB888);
                char buf[64]{};
                sprintf(buf, "D:/test_video/frame%d.jpg", cnt);
                img.save(QString(buf), "JPG");
                ++cnt;
            }
            else {
                demuxer.videoFrameQueuePop();
                break;
            }
            demuxer.videoFrameQueuePop();
        }
    }
}

int test_countFrame(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int cnt = 0;
    for (;;){
        auto frame = demuxer.videoFrameQueueFront();
        if (frame) {
            ++cnt;
            demuxer.videoFrameQueuePop();
        }
    }
}