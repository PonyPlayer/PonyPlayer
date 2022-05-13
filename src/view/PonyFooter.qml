import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "interfacefunctions.js" as IF
import HurricanePlayer
import Thumbnail
Rectangle {
    id:footer
    height: mainWindow.isFooterVisable?80:0
    visible: mainWindow.isFooterVisable
    color: "#666666"


    Label{
        id:distanceStart
        anchors.left:parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
        text:IF.videoSlideDistance(true)
        color: "white"
        font.bold: true
        lineHeight: 20
    }


    Slider{
        id:videoSlide
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: distanceStart.right
        anchors.leftMargin: 10
        anchors.right: distanceEnd.left
        anchors.rightMargin: 10
        from: 0
        to: mainWindow.endTime
        value: mainWindow.currentTime
        hoverEnabled:true
        MouseArea{
            id:previewDetector
            hoverEnabled: true
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            onEntered: preview.visible=true
            onExited: preview.visible=false
            onPositionChanged: preview.previewRequest((previewDetector.mouseX*mainWindow.endTime)/videoSlide.width)
        }
        onValueChanged: {
            mainWindow.currentTime=videoSlide.value
        }
        onPressedChanged: {
            console.log("lei fu kai use seek"+videoSlide.value)
            if(!videoSlide.pressed){
                videoArea.seek(mainWindow.currentTime)
//                preview.previewRequest(mainWindow.currentTime)
            }
        }
        Shortcut{
            sequence: "Up"
            onActivated: IF.forwardOneSecond()
        }
        Shortcut{
            sequence: "Down"
            onActivated: IF.backOneSecond()
        }
        Shortcut{
            sequence: "Left"
            onActivated: IF.backFiveSeconds()
        }
        Shortcut{
            sequence: "Right"
            onActivated: IF.forwardFiveSeconds()
        }
    }
    Rectangle {
        x:(videoSlide.x+previewDetector.mouseX-50)
        width:100
        height:80
        anchors.bottom: videoSlide.top
        anchors.bottomMargin: 10
        clip: true
        Thumbnail {
            id: preview
            player: "videoArea"
            onPreviewResponse: {
                console.log("Preview Response!!!!!!!!!!!!!!!!!!!!!!")
            }
        }

    }





    Label{
        id:distanceEnd
        anchors.right:parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
        text:IF.videoSlideDistance(false)
        color: "white"
        font.bold: true
        lineHeight: 20
    }





    Timer {
        id:timer
        interval: 1000/mainWindow.speed
        repeat: true
        running: mainWindow.isPlay
        onTriggered:IF.timerOnTriggered()
    }















    //打开文件列表
    Image {
        id: fileList
        visible: screenSize.state==="normalScreen"
        width: 20
        height: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        source: "interfacepics/FileList"
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.fileListOnClicked()
        }
    }



    //倒放
    Image {
        id: inverted
        source: "interfacepics/Inverted"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: last.left
        anchors.rightMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.invertedOnClicked()
        }
    }


    //上一个视频
    Image {
        id: last
        source: "interfacepics/Last"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: playOrPause.left
        anchors.rightMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if(mainWindow.playState === "ordered")
                    listview.currentIndex = (listview.currentIndex - 1 + listview.count)%listview.count

                else if(mainWindow.playState === "random")
                    listview.currentIndex = (listview.currentIndex - Math.floor(Math.random()*listview.count) + listview.count)%listview.count
                else;
                console.log("index:",listview.currentIndex)
                mainWindow.openFile(listModel.get(listview.currentIndex).filePath);
                mainWindow.endTime=Math.floor(videoArea.getVideoDuration())
            }
        }
    }


    //播放或是暂停
    Image {
        id: playOrPause
        width: 40
        height: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        state:  mainWindow.isPlay?"play":"pause"
        states: [
            State {
                name: "pause"
                PropertyChanges {
                    target: playOrPause
                    source: "interfacepics/Play"
                }
            },
            State {
                name: "play"
                PropertyChanges {
                    target: playOrPause
                    source: "interfacepics/Pause"
                }
            }
        ]
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.playOrPauseFunction()
        }
        Shortcut{
            sequence: "Space"
            onActivated: IF.playOrPauseFunction()
        }
    }

    //下一个视频
    Image {
        id: next
        source: "interfacepics/Next"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.left: playOrPause.right
        anchors.leftMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.nextOnClicked()
        }
    }
    //停止
    Image {
        id: cease
        source: "interfacepics/cease"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.left: next.right
        anchors.leftMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.toPause()
        }
    }
    //播放速度调整
    Image{
        id: videoSpeed
        width: 30
        height: 12
        anchors.right: playMode.left
        anchors.rightMargin: 10
        anchors.verticalCenter: playOrPause.verticalCenter
        state:  "speed1"
        states: [
            State {
                name: "speed1"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/Speed1"
                }
            },
            State {
                name: "speed2"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/Speed2"
                }
            },
            State {
                name: "speed4"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/Speed4"
                }
            },
            State {
                name: "speed8"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/Speed8"
                }
            }
        ]
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.videoSpeedOnClicked()
        }
    }
    //播放模式(顺序, 单曲循环, 随机)
    Image{
        id: playMode
        width: 20
        height: 20
        anchors.right: videoVolumn.left
        anchors.rightMargin: 10
        anchors.verticalCenter: playOrPause.verticalCenter
        state:  mainWindow.playState
        states: [
            State {
                name: "ordered"
                PropertyChanges {
                    target: playMode
                    source: "interfacepics/Ordered"
                }
            },
            State {
                name: "single"
                PropertyChanges {
                    target: playMode
                    source: "interfacepics/Single"
                }
            },
            State {
                name: "random"
                PropertyChanges {
                    target: playMode
                    source: "interfacepics/Random"
                }
            }
        ]
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.playModeOnClicked()
        }
    }
    //播放音量调节
    Item{
        id:videoVolumn
        height: 20
        width: 170
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: screenSize.right
        anchors.rightMargin: 10
        //喇叭图标, 单击可静音
        Image {
            id: speaker
            width: 20
            height: 20
            anchors.left: parent.left
            anchors.top: parent.top
            states: [
                State {
                    name: "volumn0"
                    when: mainWindow.volumn<=0
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/Volumn0"
                    }
                },
                State {
                    name: "volumn1"
                    when: mainWindow.volumn>0&&mainWindow.volumn<=0.33
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/Volumn1"
                    }
                },
                State {
                    name: "volumn2"
                    when: mainWindow.volumn>0.33&&mainWindow.volumn<=0.66
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/Volumn2"
                    }
                },
                State {
                    name: "volumn3"
                    when: mainWindow.volumn>0.66
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/Volumn3"
                    }
                }
            ]
            MouseArea{
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
                onClicked: IF.speakerOnClicked()
            }
        }
        //音量滑条, 可以调整音量
        Slider{
            id: volumnSlider
            height: 20
            width: 100
            anchors.left: speaker.right
            anchors.leftMargin: 5
            anchors.top: parent.top
            from: 0
            to: 100
            value: mainWindow.volumn*100
            onMoved: IF.volumeSliderOnMoved()
            Shortcut{
                sequence: "Ctrl+Down"
                onActivated: IF.volumnDown()
            }
            Shortcut{
                sequence: "Ctrl+Up"
                onActivated: IF.volumnUp()
            }
        }
    }
    //屏幕大小, 可调全屏或是退出全屏
    Image {
        id: screenSize
        width: 30
        height: 20
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        state:  mainWindow.isFullScreen?"fullScreen":"normalScreen"
        states: [
            State {
                name: "fullScreen"
                PropertyChanges {
                    target: screenSize
                    source: "interfacepics/NormalScreen"
                }
            },
            State {
                name: "normalScreen"
                PropertyChanges {
                    target: screenSize
                    source: "interfacepics/FullScreen"
                }
            }
        ]
        Shortcut{
            sequence: "Ctrl+F"
            onActivated: IF.screenSizeFunction()
        }

        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: IF.screenSizeFunction()
        }
    }
}
