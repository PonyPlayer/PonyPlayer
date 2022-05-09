/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Studio Components.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import HurricanePlayer
import Controller
import "./interfacefunctions.js" as IF
Window {
    function rgb(r,g,b) {
        var ret = (r << 16 | g << 8 | b)
        return ("#"+ret.toString(16)).toUpperCase();
    }

    id:mainWindow
    //是否是全屏
    property bool isFullScreen: false
    //音视频播放列表是否可视
    property bool isVideoListOpen: true
    //音视频操作栏是否可视
    property bool isFooterVisable: true
    //标题栏是否可见
    property bool isTopBarVisible: true
    //音视频是否在播放
    property bool isPlay: false
    //音视频的当前时间
    property real currentTime: 0.0
    //音视频的时间长度
    property real endTime: 0.0
    //播放倍速
    property real speed: 1.0
    //播放音量
    property real volumn: 0.25
    //播放音量的辅助(可无视)
    property real beforeMute: 0.25
    //播放时滚动条的步长(倒放时为  -1)
    property bool isInverted: false
    //播放器界面的当前宽度
    property int userWidth: 900
    //播放器界面的当前高度
    property int userHeight: 600
    //播放模式
    property string playState: "ordered"
    //亮度
    property real brightness: 0.0
    //饱和度
    property real saturation: 1.0
    //对比度
    property real contrast: 1.0


    //播放
    signal start()
    //暂停
    signal stop()
    //下一个
    signal nextOne()
    //上一个
    signal lastOne()
    //停止
    signal cease()
    //倒放
    signal inverted()
    //音量改变
    signal volumnChange(real vol)
    //播放进度改变
    signal currentTimeChange(real cur)
    //播放模式改变
    signal playModeChange(string state)
    //播放倍速改变
    signal setSpeed(real speed)
    //打开文件
    signal openFile(string path)





    width: mainWindow.userWidth
    height: mainWindow.userHeight
    minimumWidth: 750
    minimumHeight: 500
    visible: true
    title: "PonyPlayer"
    flags:  Qt.Window|Qt.FramelessWindowHint


    Shortcut {
        sequence: "F5"
        onActivated: {
            hotLoader.reload();
        }
    }

    MouseArea{
        anchors.fill: parent
        id:mainScreen
        hoverEnabled: true //默认是false
        onPositionChanged: {
            if(mainWindow.isFullScreen){
                holder.restart()
                mainWindow.isFooterVisable=true
                mainWindow.isTopBarVisible=true
            }
        }
    }
    Timer {
            id: holder
            interval: 5000
            repeat: false
            running: mainWindow.isFullScreen
            triggeredOnStart: false
            onTriggered: {
                mainWindow.isVideoListOpen=false
                mainWindow.isFooterVisable=false
                mainWindow.isTopBarVisible=false
            }
    }
    Rectangle{
        id:topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: mainWindow.isTopBarVisible?30:0
        color: "#666666"
        MouseArea { //为窗口添加鼠标事件
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton //只处理鼠标左键
            property point clickPos: "0,0"
            onPressed: { //接收鼠标按下事件
                clickPos  = Qt.point(mouse.x,mouse.y)
            }
            onPositionChanged: { //鼠标按下后改变位置
                //鼠标偏移量
                var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)

                //如果mainwindow继承自QWidget,用setPos
                mainWindow.setX(mainWindow.x+delta.x)
                mainWindow.setY(mainWindow.y+delta.y)
            }
        }
        Rectangle{
            id: innerBar
            width: 80
            height: 30
            color: "transparent"
            anchors.left:parent.left
            anchors.leftMargin:4
            Text {
                text: qsTr("PonyPlayer")
                color: "white"
                font.bold: true
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: menu.open()
                hoverEnabled: true
                onEntered: {
                    innerBar.color="#10FFFFFF"
                }
                onExited: innerBar.color="transparent"
            }
            Menu {
                id: menu
                width: 100
                topMargin: parent.height
                MenuItem {
                    text: "其他设置"
                    onTriggered:additionalSettings.show()
                }
            }
        }
        Rectangle{
            id:mainWindowClose
            width: 40
            height: 30
            color: "transparent"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            Image {
                width: 12
                height: 12
                source: "interfacepics/mainWindowClose"
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: mainWindowClose.color="red"
                onExited: mainWindowClose.color="transparent"
                onClicked: mainWindow.close()
            }
        }
        Rectangle{
            id:mainWindowReduction
            width: 40
            height: 30
            color: "transparent"
            anchors.right: mainWindowClose.left
            anchors.verticalCenter: parent.verticalCenter
            Image {
                width: 16
                height: 16
                source: "interfacepics/mainWindowReduction"
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: mainWindowReduction.color="#10FFFFFF"
                onExited: mainWindowReduction.color="transparent"
                onClicked: mainWindow.lower()
            }
        }
        Rectangle{
            id:mainWindowMinimize
            width: 40
            height: 30
            color: "transparent"
            anchors.right: mainWindowReduction.left
            anchors.verticalCenter: parent.verticalCenter
            Image {
                width: 16
                height: 16
                source: "interfacepics/mainWindowMinimize"
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: mainWindowMinimize.color="#10FFFFFF"
                onExited: mainWindowMinimize.color="transparent"
                onClicked: mainWindow.lower()
            }
        }
    }
    Timer{
        id:detechSize
        interval: 200
        repeat: true
        running:true
        onTriggered: {
            mainWindow.userWidth=mainWindow.width
            mainWindow.userHeight=mainWindow.height
        }
    }
    Dialog{
        id:operationFailedDialog
        title: "操作失败"
        width: 200
        height: 150
        standardButtons: Dialog.Ok
        Text{
            id:operationFailedDialogText
            text: "打开文件失败，请选择正确路径"
            anchors.centerIn: parent
        }
        onAccepted: console.log("Ok clicked")
    }
    AdditionalSettings{
        id:additionalSettings
    }
    Rectangle {
        id:body
        anchors.top: topBar.bottom
        anchors.left: leftSizeChange.right
        anchors.right: rightSizeChange.left
        anchors.bottom: footer.top
        //左侧播放列表栏
        Rectangle{
            id:videoList
            width: mainWindow.isVideoListOpen?200:0
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: rgb(45, 48, 50)
            Controller {
                id: mediaLibController
            }

            //列表文件操作项目
            Item{
                id:videoListOperator
                height: 20
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                //打开文件弹窗处理
                FileDialog{
                    id:fileDialog
                    title: "choose video"
                    onAccepted: IF.videoListOperatorOnAccepted()
                    onRejected: {
                        console.log("reject")
                    }
                }
                //启动打开文件
                Image{
                    id:openFile
                    width: 25
                    height: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.verticalCenter: minimize.verticalCenter
                    source: "interfacepics/FileOpener"
                    Shortcut{
                        sequence: "Ctrl+I"
                        onActivated: fileDialog.open();
                    }
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: "PointingHandCursor"
                        onClicked: fileDialog.open()
                    }
                }

                //关闭播放栏列表
                Image {
                    id: minimize
                    source: "interfacepics/Minimize"
                    width: 20
                    height: 20
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: "PointingHandCursor"
                        onClicked: {
                            mainWindow.isVideoListOpen=false
                        }
                    }
                }
            }

            // 列表代理
            Component {
                id: listDelegate
                Item {
                    id: listitem

                    height: listview.height / 10    // 每页显示 10 个
                    width: listview.width

                    MediaInfo {
                        id: mediainfowindow
                    }

                    Item {
                        id: raylayout
                        anchors.top: parent.top
                        anchors.right: deleteitem.left
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom

                        Image {
                            id: preview
                            source: "interfacepics/defaultlogo"
                            anchors.left: parent.left
                            anchors.leftMargin: parent.height*0.1
                            height: parent.height*0.8
                            width: height
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: filaname
                            text: name
                            font.bold: true
                            anchors.left : preview.right
                            anchors.leftMargin: 6
                            anchors.verticalCenter: parent.verticalCenter
                            color: rgb(173, 173, 173)
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"

                            onClicked: {
                                listview.currentIndex = index
                                console.log(listview.currentIndex)
                            }

                            onDoubleClicked: {
                                mediainfowindow.show()
                            }
                        }

                    }

                    Image {
                        id: deleteitem
                        height: preview.height*0.5
                        width: preview.width*0.5
                        source: "interfacepics/FileCloser"
                        anchors.right: parent.right
                        anchors.left: filename.left
                        anchors.leftMargin: 6
                        anchors.rightMargin: parent.height*0.1
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"

                            onClicked: {
                                console.log("Image")
                                listModel.remove(index,1)
                            }

                        }
                    }
                }

            }

            ScrollView{
                id:videoScroll
                anchors.top: videoListOperator.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ListView {
                    anchors.right: parent.right
                    anchors.left: parent.left
                    id:listview
                    focus:true
                    model: ListModel{
                        id:listModel
                         ListElement{
                             name:"7"
                             age: 7
                         }
                         ListElement{
                             name:"5"
                             age: 6
                         }
                         ListElement{
                             name:"3"
                             age: 9
                         }
                         ListElement{
                             name:"1"
                             age: 45
                         }
                     }
                    highlight:Rectangle {
                        color: "red"
                    }
                    delegate: listDelegate
                 }
            }
        }

        //视频播放区域
        SwipeView{
            id:mainArea
            orientation: Qt.Horizontal
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: videoList.left
            clip:true
            HurricanePlayer{
                id:videoArea
                clip:true
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true //默认是false
                    //onClicked: IF.videoAreaOnClicked()
                    onPositionChanged: {
                        if(mainWindow.isFullScreen){
                            holder.restart()
                            mainWindow.isFooterVisable=true
                            mainWindow.isTopBarVisible=true
                        }
                    }
                }
                onStateChanged:IF.solveStateChanged()
                Component.onCompleted: IF.mainAreaInit()
                onOpenFileResult:{
                    if(!b){
                        operationFailedDialogText.text="打开文件失败，请选择正确路径"
                        operationFailedDialog.open()
                    }
                }
            }
            Rectangle{
                id:mediaMessage
                color:"green"
            }
            Rectangle{
                id:audioWavedorm
                color:"red"
            }
        }
    }
    PonyFooter{
        id:footer
        anchors.left: leftSizeChange.right
        anchors.right: rightSizeChange.left
        anchors.bottom: downSizeChange.top
    }
    LeftSizeChange{
        id:leftSizeChange
        anchors.top:topBar.bottom
        anchors.left: parent.left
        anchors.bottom: downSizeChange.top
    }
    LeftDownSizeChange{
        id:leftDownSizeChange
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
    DownSizeChange{
        id:downSizeChange
        anchors.bottom: parent.bottom
        anchors.left: leftSizeChange.right
        anchors.right: rightSizeChange.left
    }
    RightDownSizeChange{
        id:rightDownSizeChange
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
    RightSizeChange{
        id:rightSizeChange
        anchors.right: parent.right
        anchors.top:topBar.bottom
        anchors.bottom: downSizeChange.top
    }
    Wave {
        id: wavewindow
    }
}


