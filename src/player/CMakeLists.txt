project(player)


qt_add_qml_module(${PROJECT_NAME}
        URI player
        VERSION 1.0
        STATIC
        PLUGIN_TARGET player
        SOURCES
            fireworks.hpp
            playback.hpp
            framecontroller.hpp
            hurricane.hpp
            players.cpp
            renderer.hpp
            preview.hpp
            thumbnail.hpp
        RESOURCES
            shader/vertex.vsh
            shader/fragment.fsh
            filter/kodak_kaf_2001.png
            filter/neutral-lut.png
            test.jpeg
        )

include_directories(include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        decoder
        audiosink
        )