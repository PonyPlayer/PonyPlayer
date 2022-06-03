include(FetchContent)
SET(PA_BUILD_SHARED OFF CACHE INTERNAL "Turn off shared")
SET(PA_ENABLE_DEBUG_OUTPUT OFF CACHE INTERNAL "Turn off debug output")

if(NOT NO_SSH_KEY)
    set(portaudio_GIT git@github.com:PortAudio/portaudio.git)
else()
    set(portaudio_GIT https://github.com/PortAudio/portaudio.git)
endif()

FetchContent_Declare(portaudio
    GIT_REPOSITORY ${portaudio_GIT}
    GIT_TAG        v19.7.0
    GIT_SHALLOW TRUE
)


FetchContent_MakeAvailable(portaudio)

include_directories(SYSTEM "${portaudio_SOURCE_DIR}/include")
include_directories(SYSTEM "${portaudio_SOURCE_DIR}/src/common")
target_compile_options(portaudio_static PRIVATE -w)