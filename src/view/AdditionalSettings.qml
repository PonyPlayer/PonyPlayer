import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "./interfacefunctions.js" as IF

Dialog{
    id:additionalSettings
    title: "additionalSettings"
    standardButtons: Dialog.Ok
    modal: false
    height: 180
    width: 200
    onAccepted: console.log("additional Ok clicked")
    Row{
        id:brightness
        anchors.top: parent.top
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
