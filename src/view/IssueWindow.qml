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
ApplicationWindow {
    id:issueWindow
    visible: true
    title: "程序崩溃处理"

    maximumHeight: 160
    maximumWidth: 320
    minimumWidth: 320
    minimumHeight: 160


    property string logFilePath: ""


    signal submitIssueForWeb(string issue)
    signal restartProgram()
    signal closeProgram()



    Text {
        id: tips
        anchors.topMargin: 10
        anchors.top: parent.top
        text: "抱歉程序崩溃了，请提交issue，issue为:"
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text{
        id:issueMessage
        text: "初始化"
        anchors.top: tips.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Row{
        spacing: 10
        anchors.topMargin: 30
        anchors.top: issueMessage.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Button{
            id:submitIssue
            text: "提出issue"
            onClicked: {
                issueWindow.submitIssueForWeb(issueMessage.text)
                issueWindow.close()
            }
        }
        Button{
            id:submit
            text: "打开日志文件"
        }
        Button{
            id:restartProgram
            text: "重启程序"
            onClicked: {
                issueWindow.restartProgram()
                issueWindow.close()
            }
        }
        Button{
            id:closeProgram
            text: "关闭程序"
            onClicked: {
                issueWindow.closeProgram()
                issueWindow.close()
            }
        }
    }
}

