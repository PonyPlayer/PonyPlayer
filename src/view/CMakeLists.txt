project(view)


qt_add_qml_module(${PROJECT_NAME}
        URI view
        VERSION 1.0
        STATIC
        PLUGIN_TARGET view
        QML_FILES
            main.qml
            FramedImage.qml
        RESOURCES
            img/ponyplayer-icon.png
        SOURCES
            squircle.cpp squircle.h
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick)