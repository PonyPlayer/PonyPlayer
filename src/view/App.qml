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
import PonyPlayer

Window {
    id:mainWindow
    property bool isFullScreen: false
    property bool isVideoListOpen: true
    property bool isFooterVisable: true
    property bool isPlay: false
    property int currentTime: 0
    property int endTime: 100
    //播放倍速
    property int speed: 1
    //播放音量
    property int volumn: 25
    //播放音量的辅助(可无视)
    property int beforeMute
    //到放
    property int step: 1

    width: 800
    height: 600
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



    PONYBODY{
        id:body
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
    }

    PONYFOOTER{
        id:footer
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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

