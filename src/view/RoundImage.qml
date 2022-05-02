import QtQuick
import QtQuick.Controls

Item {

    property alias source: image.source
    property alias edge: effect.edge

    Image { id: image; visible: false }

    ShaderEffect {
        id: effect
        property variant src: image
        property double edge: 0.5

        anchors.fill: parent
        vertexShader: "shader/roundImage.vert.qsb"
        fragmentShader: "shader/roundImage.frag.qsb"
    }


}

