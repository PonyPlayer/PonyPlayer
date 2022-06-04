import QtQuick
import QtQuick.Controls

Rectangle {
    id:filterItem
    width: 200
    height: 170
    property string filterName
    property string image
    property string lut
    color:"gray"
    Image {
        id: filterItemImage
        width: 200
        height: 150
        source: filterItem.image
        anchors.top: parent.top
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: mainWindow.setFilter(filterItem.lut)
        }
    }
    Text {
        id: filterItemText
        text: filterItem.filterName
        width: 200
        height: 20
        anchors.top: filterItemImage.bottom
    }
}
