import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Controls 2
import QtQuick.Layouts
import QtQuick.Dialogs

Window{
    id: additionalSettings
    title: "播放设置"
    maximumHeight: 250
    maximumWidth: 200
    minimumHeight: 250
    minimumWidth: 200
    flags: Qt.Dialog
    TabBar {
        id: bar
        width: parent.width
        anchors.top: parent.top

        TabButton {
            text: qsTr("画面")
        }
        TabButton {
            text: qsTr("音频")
        }
    }

    StackLayout {
        width: parent.width
        anchors.top: bar.bottom
        anchors.bottom: okButton.top
        anchors.topMargin: 10
        currentIndex: bar.currentIndex
        Item {
            id: sceneTab
            width: parent.width
            Row{
                id: brightness
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    width: 50
                    text: qsTr("亮度")
                }
                Slider{
                    id: brightnessSlider
                    height: 20
                    width: parent.width - 50
                    from: 0.0
                    to: 1.0
                    value: videoArea.brightness
                    onMoved: {
                        videoArea.brightness=brightnessSlider.value
                    }
                }
            }
            Row{
                id: saturation
                anchors.top: brightness.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    width: 50
                    text: qsTr("饱和度")
                }
                Slider{
                    id: saturationSlider
                    height: 20
                    width: parent.width - 50
                    from: -1.0
                    to: 1.0
                    value: videoArea.saturation
                    onMoved: {
                        videoArea.saturation=saturationSlider.value
                    }
                }
            }
            Row{
                id: contrast
                anchors.top: saturation.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    width: 50
                    text: qsTr("对比度")
                }
                Slider{
                    id: contrastSlider
                    height: 20
                    width: parent.width - 50
                    from: -2.0
                    to: 2.0
                    value: videoArea.contrast
                    onMoved: {
                        videoArea.contrast=contrastSlider.value
                    }
                }
            }
        }
        Item {
            id: audioTab
            width: parent.width
            Row{
                id: pitch
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    width: 50
                    text: qsTr("音调")
                }
                Slider{
                    id: pitchSlider
                    height: 20
                    width: parent.width - 50
                    from: 0.9
                    to: 1.2
                    value: videoArea.pitch
                    onMoved: {
                        videoArea.pitch=pitchSlider.value
                    }
                }
            }
        }
    }
    Button {
        id: resetButton
        text: "重置"
        width: 80
        height: 40
        anchors.right: okButton.left
        anchors.rightMargin: 10
        anchors.verticalCenter: okButton.verticalCenter
        onClicked: {
            videoArea.brightness=0.0
            videoArea.saturation= 1.0
            videoArea.contrast= 1.0
            videoArea.brightness=brightnessSlider.value
            videoArea.saturation=saturationSlider.value
            videoArea.contrast=contrastSlider.value
        }
    }
    Button {
        id: okButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 15
        width: 80
        height: 40
        text: "确定"
        onClicked: additionalSettings.close();
    }
}
