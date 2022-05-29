//
// Created by ColorsWind on 2022/5/13.
//

#include <utility>
#pragma once


#ifdef NDEBUG
#define NOT_IMPLEMENT_YET { throw std::runtime_error("Unsupported operation:"); }
#define ILLEGAL_STATE(msg) throw std::runtime_error(std::string("Illegal State Exception: ").append(msg))
#else
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define FILE_AND_LINE __FILE__ ":" TOSTRING(__LINE__)
#define NOT_IMPLEMENT_YET { throw std::runtime_error("Unsupported operation: " FILE_AND_LINE); }
#define ILLEGAL_STATE(msg) throw std::runtime_error(std::string("Illegal State Exception: ").append(msg).append(" : " FILE_AND_LINE))
#endif

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

    template<typename T0>
    constexpr bool checkThreadType(T0) {
        return std::is_same<T0, PonyThread>();
    }

    template<typename T0, typename ...T>
    constexpr bool checkThreadType(T0, T ...t) {
        return std::is_same<T0, PonyThread>() && checkThreadType(t...);
    }
}




#define PONY_THREAD_ANNOTATION(...) static_assert([]{using namespace PonyPlayer; return checkThreadType(__VA_ARGS__);}());

/**
 * 仅在指定线程调用可以保证线程安全
 */
#define PONY_GUARD_BY(...) PONY_THREAD_ANNOTATION(__VA_ARGS__)

/**
 * 仅在合适的时机调用可以保证线程安全. 通常通过Qt的信号机制保证.
 */
#define PONY_CONDITION(description)
#define PONY_THREAD_AFFINITY(thread) PONY_THREAD_ANNOTATION(thread)
#define PONY_THREAD_SAFE