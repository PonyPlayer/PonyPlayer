project(audiosink)

set(CPP_SOURCES private/hotplug.hpp)
qt_add_library(${PROJECT_NAME} STATIC ${CPP_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_sources(${PROJECT_NAME} PRIVATE audiosink.hpp audioformat.hpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Multimedia
    portaudio_static
    rwqueue
    sonic
    utils
    ${FFmpeg}
)

