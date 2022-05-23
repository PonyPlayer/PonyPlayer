import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Controls 2
import QtQuick.Layouts
import QtQuick.Dialogs

Window{
    id: additionalSettings
    title: "播放设置"
    maximumHeight: 300
    maximumWidth: 200
    minimumHeight: 300
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
        }
    }
    Button {
        id: okButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: 80
        height: 40
        text: "确定"
        onClicked: additionalSettings.close();
    }

}
