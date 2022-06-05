cmake_minimum_required(VERSION 3.16)
project(PonyPlayer VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(thirdparty/portaudio.cmake)
include(thirdparty/sonic.cmake)
include(thirdparty/rwqueue.cmake)
include(thirdparty/concurrentqueue.cmake)
include(thirdparty/lrctools.cmake)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=return-type -Wno-unused-function -Wno-unused-parameter")

set(FFMPEG_LIBS avformat avcodec avutil swresample swscale)
include(config/find_dependence.cmake)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Quick
        Qml
        QmlWorkerScript
        Sql
        Widgets
        Multimedia
        REQUIRED)

set(PONY_ICON ${CMAKE_SOURCE_DIR}/assets/ponyicon.png)
set(PONY_LICENSE ${CMAKE_SOURCE_DIR}/docs/LICENSE.RTF)
set(FFmpeg)
foreach(LIB IN LISTS FFMPEG_LIBS)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        find_library(FFmpeg_FOUND_${LIB} NAMES ${LIB} REQUIRED)
    else()
        find_library(FFmpeg_FOUND_${LIB} NAMES ${LIB} HINTS "${FFMPEG_PREFIX_PATH}/lib/" REQUIRED)
    endif()
    list(APPEND FFmpeg "${FFmpeg_FOUND_${LIB}}")
endforeach()

#file(GLOB_RECURSE ASSETS_FILTER CONFIGURE_DEPENDS assets/filters/**.jpg assets/filters/*.json)
find_library(FFmpeg COMPONENTS ${FFMPEG_LIBS} "${FFMPEG_PREFIX_PATH}/lib/")
set(CMAKE_MACOSX_RPATH ON)
include_directories(SYSTEM "${FFMPEG_PREFIX_PATH}/include")

add_subdirectory(src)
#add_subdirectory(package_test)