//
// Created by ColorsWind on 2022/4/30.
//
#include "demuxer2.h"

void Demuxer2::seek(int64_t us) {
    worker->seek(us);
    emit startWorker();
}

void Demuxer2::start() {
    emit startWorker();
}

