include(FetchContent)
SET(PA_BUILD_SHARED OFF CACHE INTERNAL "Turn off shared")
SET(PA_ENABLE_DEBUG_OUTPUT OFF CACHE INTERNAL "Turn off debug output")

FetchContent_Declare(portaudio
        GIT_REPOSITORY git@github.com:PortAudio/portaudio.git
        GIT_TAG        v19.7.0
        GIT_SHALLOW TRUE
        )


FetchContent_MakeAvailable(portaudio)

include_directories("${portaudio_SOURCE_DIR}/include")
include_directories("${portaudio_SOURCE_DIR}/src/common")
set_property(
        DIRECTORY ${portaudio_SOURCE_DIR}
        APPEND
        PROPERTY COMPILE_OPTIONS -w
)