//
// Created by kurisu on 2022/4/17.
//

#include "demuxer.h"

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

void test_saveFrame()
{
    Demuxer demuxer;
    demuxer.openFile("D:/test_video/video.mp4");
    demuxer.initDemuxer();
    int cnt = 0;
    for (;;){
        auto frame = demuxer.videoFrameQueueFront();
        if (frame) {
            printf("pts: %f\n", frame->pts);
            demuxer.videoFrameQueuePop();
        }
    }
}