project(view)


qt_add_qml_module(${PROJECT_NAME}
        URI view
        VERSION 1.0
        STATIC
        PLUGIN_TARGET view
        QML_FILES
            main.qml
            PonyBody.qml
            PonyFooter.qml
            interfacefunctions.js
        RESOURCES
            interfacepics/filecloser.png
            interfacepics/filelist.png
            interfacepics/fileopener.png
            interfacepics/fullscreen.png
            interfacepics/inverted.png
            interfacepics/last.png
            interfacepics/minimize.png
            interfacepics/next.png
            interfacepics/normalscreen.png
            interfacepics/ordered.png
            interfacepics/pause.png
            interfacepics/play.png
            interfacepics/random.png
            interfacepics/single.png
            interfacepics/speed1.png
            interfacepics/speed2.png
            interfacepics/speed4.png
            interfacepics/speed8.png
            interfacepics/volumn0.png
            interfacepics/volumn1.png
            interfacepics/volumn2.png
            interfacepics/volumn3.png
        )
