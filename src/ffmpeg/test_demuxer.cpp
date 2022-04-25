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

    const char outFileName[] = "D:/test_video/test.pcm";
    FILE *file = fopen(outFileName, "w+b");

    int cnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            auto sample = demuxer.getSample(true);
            if (sample.valid) {

                fwrite(sample.data, 1, sample.len, file);

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

    int ret = demuxer.openFile("D:/test_video/577099243-1-208.mp4");

    int cnt = 0;
    bool change = false;
    bool getChanged = false;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            ++cnt;
            if (cnt % 2)
                demuxer.seek(30 * 1000000);
            else
                demuxer.openFile("D:/test_video/577099243-1-208.mp4");

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            auto pic = demuxer.getPicture(true);
            if (pic.isValid()) {
                demuxer.popPicture();
                pic.free();
                printf("get video\n");
            }

            auto sample = demuxer.getSample(true);
            if (sample.valid) {
                while (audioOutput->bytesFree() < sample.len)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                streamOut->write(reinterpret_cast<const char *>(sample.data), sample.len);
                demuxer.popSample();
                sample.free();
                printf("get audio\n");
            } else {
                // printf("invalid\n");
            }
            if (quit) {
                break;
            }
        }
    });

    /*
    demuxer.openFile("D:/test_video/577099243-1-208.mp4");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (int i = 0; i < 50; i++) {
        change = true;
        while (!getChanged)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (i % 2)
            demuxer.openFile("D:/test_video/577099243-1-208.mp4");
        else
            demuxer.seek(60 * 1000000);
        change = false;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
*/
    std::this_thread::sleep_for(std::chrono::seconds(50));
    quit = true;
    demuxer.quit();
    worker.join();
}

void test_quit(std::string filename) {
    Demuxer demuxer;
    demuxer.openFile(filename);
    printf("%f %f\n", demuxer.videoDuration(), demuxer.audioDuration());
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
                sample.free();
                printf("%d %d\n", audioCnt, sample.len);
            }
            if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    quit = true;
    demuxer.quit();
    worker.join();
}

void test_pause(std::string filename, bool halfQuit) {
    Demuxer demuxer;
    demuxer.openFile(filename);
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
    } else {
        demuxer.resume();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        demuxer.quit();
        quit = true;
    }
    worker.join();
}

void test_eof(std::string filename) {
    Demuxer demuxer;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    demuxer.quit();
}

void test_seek(std::string filename) {
    Demuxer demuxer;

    bool seekWait{};
    bool ackWait{};

    int videoCnt = 0, audioCnt = 0;
    bool quit = false;
    auto worker = std::thread([&]() {
        for (;;) {
            demuxer.openFile(filename);

            auto picture = demuxer.getPicture(true);
            auto sample = demuxer.getSample(true);
            if (picture.isValid()) {
                demuxer.popPicture();
                ++videoCnt;
                //printf("video: %d %f\n", videoCnt, picture.getPTS());
                picture.free();
            }
            if (sample.valid) {
                demuxer.popSample();
                ++audioCnt;
                printf("audio: %d %f\n", audioCnt, sample.getPTS());
            }
            if (quit) {
                break;
            }
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    quit = true;
    demuxer.quit();
    worker.join();
}