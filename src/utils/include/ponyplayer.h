//
// Created by ColorsWind on 2022/5/13.
//

#include <utility>
#pragma once

#define NOT_IMPLEMENT_YET {throw std::runtime_error("Unsupported operation.");}

#define PONY_THREAD_ANNOTATION(thread) static_assert(static_cast<PonyPlayer::PonyThread>(thread));
#define PONY_GUARD_BY(thread) PONY_THREAD_ANNOTATION(PonyPlayer::thread)
#define PONY_THREAD_AFFINITY(thread) PONY_THREAD_ANNOTATION(PonyPlayer::thread)
#define PONY_THREAD_SAFE
namespace PonyPlayer {
    using PonyThread = const char*;
    constexpr PonyThread PLAYBACK = "PlaybackThread";
    constexpr PonyThread DECODER  = "DecoderThread";
    constexpr PonyThread MAIN     = "MainThread";
    constexpr PonyThread RENDER   = "RenderThread";
    constexpr PonyThread PREVIEW  = "PreviewThread";
    constexpr PonyThread FRAME    = "FrameControllerThread";

    constexpr PonyThread ANY  = "__AnyThread";
    constexpr PonyThread SELF = "__SelfThread";
}