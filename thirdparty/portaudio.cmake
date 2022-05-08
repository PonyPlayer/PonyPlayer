include(FetchContent)

FetchContent_Declare(portaudio
        GIT_REPOSITORY git@github.com:PortAudio/portaudio.git
        GIT_TAG        v19.7.0
        )

OPTION(PA_BUILD_SHARED OFF)
FetchContent_MakeAvailable(portaudio)

include_directories("${portaudio_SOURCE_DIR}/include")
include_directories("${portaudio_SOURCE_DIR}/src/common")
set_property(
        DIRECTORY ${portaudio_SOURCE_DIR}
        APPEND
        PROPERTY COMPILE_OPTIONS -w
)