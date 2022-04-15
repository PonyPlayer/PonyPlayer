project(render)



qt_add_qml_module(${PROJECT_NAME}
        URI hurricane
        VERSION 1.0
        STATIC
        PLUGIN_TARGET hurricane
        SOURCES
        hurricane.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick)