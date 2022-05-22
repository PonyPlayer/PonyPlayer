import QtQuick.Controls.Material
import QtQml.Models
import Qt.labs.platform

MenuBar {
    id: dbus

    Menu {
        id: fileMenu
        title: qsTr("文件")

        MenuItem {
            text: qsTr("打开文件")
            onTriggered: fileDialog.open()
        }

        MenuItem {
            text: qsTr("最近打开的文件")
        }
    }

    Menu {
        id: scene
        title: qsTr("画面")

        MenuItem {
            text: qsTr("色彩")
            onTriggered: additionalSettings.show()
        }

        MenuItem {
            text: qsTr("滤镜")
        }
    }

    Menu {
        title: qsTr("音频")

        Menu {
            id: audioMenu
            title: qsTr("输出设备")
            Instantiator {
                id: audioInstantiator
                model: videoArea.audioDeviceList
                delegate: MenuItem {
                    text: model.modelData
                    onTriggered: videoArea.setSelectedAudioOutputDevice(model.modelData)
                }
                onObjectAdded: audioMenu.insertItem(index, object)
                onObjectRemoved: audioMenu.removeItem(object)
           }
        }

        MenuItem {
            text: qsTr("音轨")
        }
    }
}
