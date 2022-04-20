project(view)


qt_add_qml_module(${PROJECT_NAME}
        URI view
        VERSION 1.0
        STATIC
        PLUGIN_TARGET view
        QML_FILES
            main.qml
            App.qml
            PonyBody.qml
            PonyFooter.qml
        RESOURCES
            PonyPics/FileCloser.png
            PonyPics/FileList.png
            PonyPics/FileOpener.png
            PonyPics/FullScreen.png
            PonyPics/Inverted.png
            PonyPics/Last.png
            PonyPics/Minimize.png
            PonyPics/Next.png
            PonyPics/NormalScreen.png
            PonyPics/Ordered.png
            PonyPics/Pause.png
            PonyPics/Play.png
            PonyPics/Random.png
            PonyPics/Single.png
            PonyPics/Speed1.png
            PonyPics/Speed2.png
            PonyPics/Speed4.png
            PonyPics/Speed8.png
            PonyPics/Volumn0.png
            PonyPics/Volumn1.png
            PonyPics/Volumn2.png
            PonyPics/Volumn3.png
        )