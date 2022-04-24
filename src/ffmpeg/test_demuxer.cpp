//
// Created by kurisu on 2022/4/19.
//

#include "demuxer.h"

#include <QMediaDevices>
#include <QAudioFormat>
#include <QAudioSink>
#include <QAudioDevice>

void test_saveFrame(std::string filename, int n) {

}

void test_savePCM(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();

    const char outFileName[] = "D:/test_video/test.pcm";
    FILE *file=fopen(outFileName,"w+b");

    int cnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto sample = demuxer.getSample(true);
            if (sample.valid) {

                fwrite(sample.data,1,sample.len,file);

                printf("%d\n", ++cnt);
                demuxer.popSample();
            }
            if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(3));
    quit = true;
    demuxer.quit();
    worker.join();
}

void test_audio(std::string filename) {
    QAudioDevice info(QMediaDevices::defaultAudioOutput());
    QAudioSink *audioOutput;
    QIODevice *streamOut;
    QAudioFormat audioFmt;
    audioFmt.setSampleRate(44100);
    audioFmt.setChannelCount(2);
    audioFmt.setSampleFormat(QAudioFormat::Int16);

    audioOutput = new QAudioSink(audioFmt);
    audioOutput->setVolume(100);
    audioOutput->setBufferSize(192000 * 2);

    streamOut = audioOutput->start();

    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();

    int cnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto sample = demuxer.getSample(true);
            if (sample.valid) {

                while (audioOutput->bytesFree() < sample.len)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                streamOut->write(reinterpret_cast<const char *>(sample.data), sample.len);

                printf("%d\n", ++cnt);
                demuxer.popSample();
            }
            if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(10));
    quit = true;
    demuxer.quit();
    worker.join();
}

void test_quit(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    demuxer.initDemuxer();
    int videoCnt = 0, audioCnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto picture = demuxer.getPicture(true);
            auto sample = demuxer.getSample(true);
            if (picture.isValid()) {
                demuxer.popPicture();
                ++videoCnt;
                printf("%d %d\n", videoCnt, picture.getHeight());
                picture.free();
            }
            if (sample.valid) {
                demuxer.popSample();
                ++audioCnt;
                printf("%d %d\n", audioCnt, sample.len);
            }
            if (quit) {
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
    int videoCnt = 0, audioCnt = 0;
    auto worker = std::thread([&]() {
        for (;;) {
            auto picture = demuxer.getPicture(true);
            auto sample = demuxer.getSample(true);
            if (picture.isValid()) {
                demuxer.popPicture();
                ++videoCnt;
                printf("%d %d\n", videoCnt, picture.getHeight());
                picture.free();
            }
            if (sample.valid) {
                demuxer.popSample();
                ++audioCnt;
                printf("%d %d\n", audioCnt, sample.len);
            }
            if (quit) {
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