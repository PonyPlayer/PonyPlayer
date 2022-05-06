project(quickitem)


qt_add_qml_module(${PROJECT_NAME}
        URI quickitem
        VERSION 1.0
        STATIC
        PLUGIN_TARGET quickitem
        SOURCES
            playback.hpp
            framecontroller.hpp
            hurricane.hpp
        )


target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Multimedia
        Qt::Quick)