project(decoder)

set(CPP_SOURCES
        decoder.hpp dispatcher.hpp frame.hpp helper.hpp demuxer.hpp)

qt_add_library(${PROJECT_NAME} STATIC
        ${CPP_SOURCES})
#target_sources(${PROJECT_NAME} PRIVATE demuxer.hpp)
include_directories(include)
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick
        utils
        ${FFmpeg})

