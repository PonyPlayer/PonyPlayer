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
import "./interfacefunctions.js" as IF
Window {
    id:mainWindow
    //是否是全屏
    property bool isFullScreen: false
    //音视频播放列表是否可视
    property bool isVideoListOpen: true
    //音视频操作栏是否可视
    property bool isFooterVisable: true
    //音视频是否在播放
    property bool isPlay: false
    //音视频的当前时间
    property int currentTime: 0
    //音视频的时间长度
    property int endTime: 100
    //播放倍速
    property real speed: 1.0
    //播放音量
    property int volumn: 25
    //播放音量的辅助(可无视)
    property int beforeMute
    //播放时滚动条的步长(倒放时为  -1)
    property int step: 1
    //播放器界面的当前宽度
    property int userWidth: 800
    //播放器界面的当前高度
    property int userHeight: 600
    //播放模式
    property string playState: "ordered"


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
    signal volumnChange(int vol)
    //播放进度改变
    signal currentTimeChange(int cur)
    //播放模式改变
    signal playModeChange(string state)
    //播放倍速改变
    signal setSpeed(real speed)
    //打开文件
    signal openFile(string path)





    width: mainWindow.userWidth
    height: mainWindow.userHeight
    minimumWidth: 650
    minimumHeight: 500
    visible: true
    title: "PonyPlayer"


//    Timer {
//            id: holder
//            interval: 5000
//            repeat: false
//            running: mainWindow.isFullScreen
//            triggeredOnStart: false
//            onTriggered: {
//                mainWindow.isVideoListOpen=false
//                mainWindow.isFooterVisable=false
//            }

//        }
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


    PonyBody{
        id:body
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
    }

    PonyFooter{
        id:footer
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Component.onCompleted: {
            mainWindow.setSpeed.connect(femo)
            mainWindow.start.connect(statata)
        }
        function statata(){console.log("start")}
        function femo(spe){
            console.log(spe)
        }
    }
//    MouseArea{
//        anchors.fill: parent
//        id:mainScreen
//        hoverEnabled: true //默认是false
//        propagateComposedEvents: true
//        onPositionChanged: {
//            if(mainWindow.isFullScreen){
//                holder.restart()
//                mainWindow.isVideoListOpen=true
//                mainWindow.isFooterVisable=true
//            }
//            console.log("xxxxx")
//        }
//    }
}


