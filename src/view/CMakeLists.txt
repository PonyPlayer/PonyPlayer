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
        )