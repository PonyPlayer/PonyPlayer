import QtQuick
import QtQuick.Controls

Rectangle {
    id:filterItem
    width: 200
    height: 170
    property string filterName: ""
    property string image
    property string lut
    signal sentFilterLut(string lut)
    Dialog{
        id:dialog
        title: ("是否选择滤镜: "+filterItem.filterName)
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: filterItem.sentFilterLut(filterItem.lut)
    }
    Image {
        id: filterItemImage
        width: 200
        height: 150
        source: filterItem.image
        anchors.top: parent.top
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: dialog.open()
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
