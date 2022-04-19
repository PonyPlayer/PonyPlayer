project(ffmpeg)

set(CPP_SOURCES
        demuxer.cpp test_demuxer.cpp)

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick
        ${FFmpeg})

