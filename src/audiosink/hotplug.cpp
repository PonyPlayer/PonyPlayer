#include "hotplug.h"
#include "audiosink.hpp"

HotPlugDetector::HotPlugDetector(PonyAudioSink *audioSink_) {
    audioSink = audioSink_;
    connect(&qMediaDevices, &QMediaDevices::audioOutputsChanged, audioSink, &PonyAudioSink::audioOutputDevicesChanged);
}

std::atomic_bool PonyAudioSink::paInitialized = false;


std::mutex PonyAudioSink::paStreamLock;