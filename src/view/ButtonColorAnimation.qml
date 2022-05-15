import QtQuick
Item {
    property alias target: animation.target
    property alias to: animation.to
    PropertyAnimation {id: animation; properties: "color"; duration: 100}
    property alias ani: animation
}