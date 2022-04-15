project(render)


qt_add_qml_module(${PROJECT_NAME}
        URI Hurricane
        VERSION 1.0
        STATIC
        PLUGIN_TARGET hurricane
        SOURCES
        hurricane.cpp)

target_sources(render PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hurricane.h)


target_link_libraries(render PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick)