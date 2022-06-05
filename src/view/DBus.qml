import QtQuick.Controls.Material
import Qt.labs.platform
import QtQml.Models
import QtQuick.Controls

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
            text: qsTr("打开链接")
            onTriggered: openLinkDialog.open()
        }

        Menu {
            id: recentMenu
            title: qsTr("最近打开的文件")
            Instantiator {
                id: recentInstantiator
                model: mediaLibController.recentFiles
                delegate: MenuItem {
                    text: model.modelData[0]
                    checked: false
                    onTriggered: videoArea.openFile(model.modelData[1])
                }
                onObjectAdded: recentMenu.insertItem(index, object)
                onObjectRemoved: recentMenu.removeItem(object)
            }
        }
    }

    Menu {
        id: scene
        title: qsTr("画面")

        MenuItem {
            text: qsTr("播放设置")
            onTriggered: additionalSettings.show()
        }

        MenuItem {
            text: qsTr("滤镜选择")
            onTriggered: filterswindow.show()
        }

        MenuSeparator { }

        MenuItem {
            text: qsTr("保持比例")
            checked: videoArea.keepFrameRate
            onTriggered: {
                videoArea.keepFrameRate = true
            }
        }

        MenuItem {
            text: qsTr("拉伸画面")
            checked: !videoArea.keepFrameRate
            onTriggered: {
                videoArea.keepFrameRate = false
            }
        }
    }

    Menu {
        id: playMenu
        title: qsTr("播放")
        Menu {
            property var speed: [0.5, 1.0, 1.25, 1.5, 2.0, 4.0, 8.0]
            id: speedMenu
            title: qsTr("倍速")
            Instantiator {
                id: speedInstantiator
                model: speedMenu.speed
                delegate: MenuItem {
                    text: model.modelData
                    checked: text==videoArea.speed
                    onTriggered: videoArea.setSpeed(parseFloat(text));
                }
                onObjectAdded: speedMenu.insertItem(index, object)
                onObjectRemoved: speedMenu.removeItem(index, object)
            }

        }

        MenuItem {
            text: qsTr("倒放")
            checked: videoArea.backwardStatus
            onTriggered: videoArea.toggleBackward()
        }

        Menu {
            property var status: [qsTr("开启"),qsTr("关闭")]
            id: serializeMenu
            title: qsTr("自动连播")
            Instantiator {
                id: serializeInstantiator
                model: serializeMenu.status
                delegate: MenuItem {
                    text: model.modelData
                    checked: text===(mainWindow.serialize ? qsTr("开启"): qsTr("关闭"))
                    onTriggered: { mainWindow.serialize = (text === qsTr("开启") ? true : false); console.log("[DDD]:",mainWindow.serialize); }
                }
                onObjectAdded: serializeMenu.insertItem(index, object)
                onObjectRemoved: serializeMenu.removeItem(index, object)
            }

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
                    checked: text === videoArea.currentOutputDevice
                    onTriggered: videoArea.setCurrentOutputDevice(model.modelData)
                }
                onObjectAdded: audioMenu.insertItem(index, object)
                onObjectRemoved: audioMenu.removeItem(object)
            }
        }
        Menu {
            id: trackMenu_
            title: qsTr("音轨")
            Instantiator {
                id: trackInstantiator
                model: videoArea.tracks
                delegate: MenuItem {
                    text: model.modelData
                    checked: index === videoArea.track
                    onTriggered: videoArea.setTrack(model.modelIndex)
                }
                onObjectAdded: trackMenu_.insertItem(index, object)
                onObjectRemoved: trackMenu_.removeItem(index, object)
            }
        }
    }
}
