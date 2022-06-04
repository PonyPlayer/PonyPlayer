//
// Created by kurisu on 2022/6/4.
//

#include <gtest/gtest.h>
#include "demuxer.hpp"
#include "private/previewer.hpp"

Demuxer* getDemuxer(const std::string& filename) {
    auto* demuxer = new Demuxer{nullptr};
    demuxer->openFile(filename);
    demuxer->setOutputFormat(demuxer->getInputFormat());
    return demuxer;
}

void getFrame(Demuxer* demuxer, qreal seekTo = 0.0, int n_frames = 10) {
    demuxer->seek(seekTo);
    demuxer->flush();
    demuxer->start();
    auto thread = std::thread([&](){demuxer->test_onWork();});
    for (int i = 0; i < n_frames; i++) {
        auto pict = demuxer->getPicture();
        auto audio = demuxer->getSample();
        if (pict.isValid() && audio.isValid())
            std::cerr << pict.getPTS() << " " << audio.getPTS() << std::endl;
        else
            break;
    }
    demuxer->pause();
    thread.join();
}


TEST(decoder_test, test_forward_getFrame) {
    auto demuxer = getDemuxer("D:/test_video/dj.mp4");
    std::cerr << "---------5.0---------" << std::endl;
    getFrame(demuxer, 5.0, 3);
    std::cerr << "---------10.0---------" << std::endl;
    getFrame(demuxer, 10.0, 3);
    std::cerr << "---------15.0---------" << std::endl;
    getFrame(demuxer, 15.0, 3);
    demuxer->close();
}

TEST(decoder_test, test_backward_getFrame) {
    auto demuxer = getDemuxer("D:/test_video/dj.mp4");
    demuxer->backward();
    std::cerr << "---------5.0---------" << std::endl;
    getFrame(demuxer, 5.0, 3);
    std::cerr << "---------10.0---------" << std::endl;
    getFrame(demuxer, 10.0, 3);
    std::cerr << "---------15.0---------" << std::endl;
    getFrame(demuxer, 15.0, 3);
    demuxer->close();
}

TEST(decoder_test, test_preview) {
    Previewer previewer("D:/test_video/dj.mp4", nullptr);
    auto pict = previewer.previewRequest(5.0);
    std::cerr << "---------5.0---------" << std::endl;
    std::cerr << pict.getPTS() << std::endl;

    pict = previewer.previewRequest(10.0);
    std::cerr << "---------10.0---------" << std::endl;
    std::cerr << pict.getPTS() << std::endl;

    pict = previewer.previewRequest(15.0);
    std::cerr << "---------15.0---------" << std::endl;
    std::cerr << pict.getPTS() << std::endl;

    pict = previewer.previewRequest(16.0);
    std::cerr << "---------16.0---------" << std::endl;
    std::cerr << pict.getPTS() << std::endl;

    pict = previewer.previewRequest(17.0);
    std::cerr << "---------17 .0---------" << std::endl;
    std::cerr << pict.getPTS() << std::endl;
}