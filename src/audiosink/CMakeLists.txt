project(audiosink)

set(CPP_SOURCES audiosink.hpp audioformat.hpp)

qt_add_library(${PROJECT_NAME} STATIC ${CPP_SOURCES})
include_directories(include)

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})
target_sources(${PROJECT_NAME} PRIVATE audiosink.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Quick
        portaudio_static
        rwqueue
        sonic
        decoder
        )

