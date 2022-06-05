import QtQuick
Item {
    property alias image_width: image.width
    property alias image_height: image.height
    property alias imageSource: image.source
    property alias hoverColor: hoverAnimation.to
    property alias normalColor: exitAnimation.to
    property alias color: rect.color
    property alias mouseArea: mouseArea
    Rectangle {
        id: rect
        anchors.fill: parent
        Image {
            id: image
            anchors.centerIn: parent
        }
        MouseArea{
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {exitAnimation.ani.stop();hoverAnimation.ani.start();}
            onExited: {hoverAnimation.ani.stop();exitAnimation.ani.start();}
        }
        ButtonColorAnimation {id: hoverAnimation; target: rect;}
        ButtonColorAnimation {id: exitAnimation; target: rect;}
    }

}