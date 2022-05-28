project(wave)


qt_add_qml_module(${PROJECT_NAME}
        URI wave
        VERSION 1.0
        STATIC
        PLUGIN_TARGET wave
        SOURCES
            wave.hpp
            lyrics.cpp
        RESOURCES
        )

target_include_directories(wave PUBLIC include)

target_sources(wave PRIVATE
        include/lyrics.h
        )


target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        decoder
        player
        audiosink
        lrctools
        )