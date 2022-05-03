project(ffmpeg)

set(CPP_SOURCES
        dispatcher.cpp decoder.cpp)

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/demuxer2.h ${CMAKE_SOURCE_DIR}/include/blocking_queue.h)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Multimedia
        utils
        ${FFmpeg})

