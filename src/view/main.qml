import QtQuick
import view 1.0
import QtQuick.Dialogs
import QtQuick.Controls
import Hurricane 1.0
import QtQuick.Window


ApplicationWindow {
    id: window
    width: 320
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("文件(&F)")
            Action {
                text: qsTr("&打开(&O)...")
                onTriggered: {
                    fileDialog.open()
                }
            }
        }
    }


    Hurricane {
        id: squircle
        anchors.fill: parent
    }

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "The background here is a squircle rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
    FileDialog{
        id: fileDialog;
        title: "Please choose a file";
        onAccepted: {
            squircle.loadImage(this.currentFile);
        }
    }

}
