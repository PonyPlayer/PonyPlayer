cmake_minimum_required(VERSION 3.16)
project(PonyPlayer VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wno-unused-parameter -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow")

set(FFMPEG_LIBS avformat avcodec avutil swresample)
include(cmake/find_dependence.cmake)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Quick
        Qml
        QmlWorkerScript
        REQUIRED)

set(FFmpeg)
foreach(LIB IN LISTS FFMPEG_LIBS)
    find_library(FFmpeg_FOUND_${LIB} NAMES ${LIB} HINTS "${FFMPEG_PREFIX_PATH}/lib/" REQUIRED)
    list(APPEND FFmpeg "${FFmpeg_FOUND_${LIB}}")
endforeach()
find_library(FFmpeg COMPONENTS ${FFMPEG_LIBS} "${FFMPEG_PREFIX_PATH}/lib/")

include_directories(${FFMPEG_PREFIX_PATH}/include)

include_directories(include)
add_subdirectory(src)
