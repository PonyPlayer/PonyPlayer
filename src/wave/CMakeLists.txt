project(wave)


qt_add_qml_module(${PROJECT_NAME}
        URI wave
        VERSION 1.0
        STATIC
        PLUGIN_TARGET wave
        SOURCES
        wave.cpp
        RESOURCES
        )

target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/wave.h)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick)