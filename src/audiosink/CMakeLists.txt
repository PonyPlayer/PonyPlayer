project(audiosink)

set(CPP_SOURCES audiosink.hpp include/audioformat.hpp)

qt_add_library(${PROJECT_NAME} STATIC ${CPP_SOURCES})
include_directories(include)


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE audiosink.hpp hotplug.cpp include/hotplug.h)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Multimedia
    portaudio_static
    rwqueue
    sonic
    decoder
    ${FFmpeg}
)

