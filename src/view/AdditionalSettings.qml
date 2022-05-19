import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Window{
    id:additionalSettings
    title: "additionalSettings"
    maximumHeight: 120
    maximumWidth:170
    minimumHeight: 120
    minimumWidth: 170
    flags: Qt.Dialog
    Row{
        id:brightness
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        Text {
            width: 50
            text: qsTr("亮度")
        }
        Slider{
            id:brightnessSlider
            height: 20
            width: 100
            from: 0.0
            to:1.0
            value: mainWindow.brightness
            onMoved: {
                mainWindow.brightness=brightnessSlider.value
            }
        }
    }
    Row{
        id:saturation
        anchors.top: brightness.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        Text {
            width: 50
            text: qsTr("饱和度")
        }
        Slider{
            id:saturationSlider
            height: 20
            width: 100
            from: -1.0
            to:1.0
            value: mainWindow.saturation
            onMoved: {
                mainWindow.saturation=saturationSlider.value
            }
        }
    }
    Row{
        id:contrast
        anchors.top: saturation.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        Text {
            width: 50
            text: qsTr("对比度")
        }
        Slider{
            id:contrastSlider
            height: 20
            width: 100
            from: -2.0
            to:2.0
            value: mainWindow.contrast
            onMoved: {
                mainWindow.contrast=contrastSlider.value
            }
        }
    }

}
