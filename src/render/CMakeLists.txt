project(render)


qt_add_qml_module(${PROJECT_NAME}
        URI render
        VERSION 1.0
        STATIC
        PLUGIN_TARGET render
        SOURCES
        hurricane.cpp
        RESOURCES
            shader/vertex.vsh
            shader/fragment.fsh
            test.jpeg
        )

target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hurricane.h)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        ffmpeg
        )