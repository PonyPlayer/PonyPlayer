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
import QtQuick.Controls.Material
ApplicationWindow {
    id: issueWindow
    visible: true
    title: "PonyPlayer Crash Reporter"
    Material.theme: Material.System
    Material.accent: Material.Grey

    maximumHeight: 300
    maximumWidth: 400
    minimumWidth: 400
    minimumHeight: 300


    property string logFilePath: ""


    Rectangle{
        anchors.fill:parent
        color:"#666666"
        Text {
                id: tips
                color:"white"
                font.bold: true
                anchors.topMargin: 10
                anchors.top: parent.top
                text: "抱歉程序崩溃了, 我们已经记录了相关错误信息, 请附上日志提交反馈, 谢谢!"
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text{
                id:issueMessage
                text: crash_reporter.crashMessage
                color:"white"
                font.bold: true
                font.italic: true
                anchors.centerIn: parent
            }
            Row{
                spacing: 10
                anchors.bottomMargin: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10
                Button{
                    id: submitIssue
                    text: "提出Issue"
                    onClicked: {
                        crash_reporter.openIssueBrowser()
                    }
                }
                Button{
                    id: copyLogPath
                    text: "复制日志路径"
                    onClicked: {
                        crash_reporter.copyLogPath()
                    }
                }
                Button{
                    id: restartApplication
                    text: "重启程序"
                    onClicked: {
                        crash_reporter.restart()
                    }
                }
            }
    }
}

