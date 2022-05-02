include(FetchContent)
FetchContent_Declare(portaudio
        GIT_REPOSITORY git@github.com:PortAudio/portaudio.git
        GIT_TAG        v19.7.0
        )
FetchContent_MakeAvailable(portaudio)
include_directories("${portaudio_SOURCE_DIR}/include")
include_directories("${portaudio_SOURCE_DIR}/src/common")