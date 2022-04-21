project(quickitem)


qt_add_qml_module(${PROJECT_NAME}
        URI quickitem
        VERSION 1.0
        STATIC
        PLUGIN_TARGET quickitem
        SOURCES
            player.cpp
        )

target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/quickitem.h)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick)