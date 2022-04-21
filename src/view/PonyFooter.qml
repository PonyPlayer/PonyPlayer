import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "interfacefunctions.js" as IF
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
        text:((mainWindow.currentTime)>3600?parseInt((mainWindow.currentTime)/3600)+":":"")+(((mainWindow.currentTime)>60)?((parseInt((mainWindow.currentTime)/60)%60)>10?(parseInt((mainWindow.currentTime)/60)%60+":"):('0'+(parseInt((mainWindow.currentTime)/60)%60))+":"):"")+(((mainWindow.currentTime)%60)<10?'0'+(mainWindow.currentTime)%60:(mainWindow.currentTime)%60)
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
        handle: Rectangle {
                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: parent.pressed ? "#f0f0f0" : "#f6f6f6"
                border.color: "#bdbebf"
            }
        onValueChanged: {
            if(mainWindow.currentTime<0){
                mainWindow.currentTime=0
                videoSlide.value=0
            }
            else if(mainWindow.currentTime>=mainWindow.endTime){
                mainWindow.currentTime=0
                videoSlide.value=0
                mainWindow.isPlay=false
                mainWindow.pause()
            }
            mainWindow.currentTime=videoSlide.value
            mainWindow.currentTimeChange(videoSlide.value)
        }

        onPressedChanged: {
            if(mainWindow.isPlay){
                mainWindow.isPlay=false
                mainWindow.pause()
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





    Label{
        id:distanceEnd
        anchors.right:parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
        text:((mainWindow.endTime-mainWindow.currentTime)>3600?parseInt((mainWindow.endTime-mainWindow.currentTime)/3600)+":":"")+(((mainWindow.endTime-mainWindow.currentTime)>60)?((parseInt((mainWindow.endTime-mainWindow.currentTime)/60)%60)>10?(parseInt((mainWindow.endTime-mainWindow.currentTime)/60)%60+":"):('0'+(parseInt((mainWindow.endTime-mainWindow.currentTime)/60)%60))+":"):"")+(((mainWindow.endTime-mainWindow.currentTime)%60)<10?'0'+(mainWindow.endTime-mainWindow.currentTime)%60:(mainWindow.endTime-mainWindow.currentTime)%60)
        color: "white"
        font.bold: true
        lineHeight: 20
    }





    Timer {
        id:timer
        interval: 1000/mainWindow.speed
        repeat: true
        running: mainWindow.isPlay
        onTriggered:{
            if(mainWindow.currentTime<=0&&mainWindow.step==-1){
                mainWindow.isPlay=false
            }
            else if(mainWindow.currentTime>=mainWindow.endTime&&mainWindow.step==1){
                mainWindow.isPlay=false
            }

            else{
                mainWindow.currentTime=mainWindow.currentTime+mainWindow.step
                videoSlide.value=currentTime
            }
        }
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
        source: "interfacepics/filelist"
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if(mainWindow.isVideoListOpen){
                    mainWindow.isVideoListOpen=false
                }
                else{
                    mainWindow.isVideoListOpen=true
                }

            }
        }
    }









    //倒放
    Image {
        id: inverted
        source: "interfacepics/inverted"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: last.left
        anchors.rightMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if(mainWindow.step===1){
                    mainWindow.step=-1
                }
                else{
                    mainWindow.step=1
                }
                mainWindow.inverted(mainWindow.step)
            }
        }
    }


    //上一个视频
    Image {
        id: last
        source: "interfacepics/last"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.right: playOrPause.left
        anchors.rightMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                mainWindow.lastOne()
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
                    source: "interfacepics/play"
                }
            },
            State {
                name: "play"
                PropertyChanges {
                    target: playOrPause
                    source: "interfacepics/pause"
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
        source: "interfacepics/next"
        width: 30
        height: 30
        anchors.verticalCenter: playOrPause.verticalCenter
        anchors.left: playOrPause.right
        anchors.leftMargin: 5
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                mainWindow.nextOne()
            }
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
                    source: "interfacepics/speed1"
                }
            },
            State {
                name: "speed2"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/speed2"
                }
            },
            State {
                name: "speed4"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/speed4"
                }
            },
            State {
                name: "speed8"
                PropertyChanges {
                    target: videoSpeed
                    source: "interfacepics/speed8"
                }
            }
        ]
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if(videoSpeed.state==="speed1"){
                    videoSpeed.state="speed2"
                    mainWindow.speed=2
                }
                else if(videoSpeed.state==="speed2"){
                    videoSpeed.state="speed4"
                    mainWindow.speed=4
                }
                else if(videoSpeed.state==="speed4"){
                    videoSpeed.state="speed8"
                    mainWindow.speed=8
                }
                else{
                    videoSpeed.state="speed1"
                    mainWindow.speed=1
                }
                mainWindow.speedChange(mainWindow.speed)
            }
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
                    source: "interfacepics/ordered"
                }
            },
            State {
                name: "single"
                PropertyChanges {
                    target: playMode
                    source: "interfacepics/single"
                }
            },
            State {
                name: "random"
                PropertyChanges {
                    target: playMode
                    source: "interfacepics/random"
                }
            }
        ]
        MouseArea{
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                if(playMode.state==="ordered"){
                    playMode.state="single"
                }
                else if(playMode.state==="single"){
                    playMode.state="random"
                }
                else{
                    playMode.state="ordered"
                }
                mainWindow.playModeChange(playMode.state)
            }
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
                    when: mainWindow.volumn===0
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/volumn0"
                    }
                },
                State {
                    name: "volumn1"
                    when: mainWindow.volumn>0&&mainWindow.volumn<=33
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/volumn1"
                    }
                },
                State {
                    name: "volumn2"
                    when: mainWindow.volumn>33&&mainWindow.volumn<=66
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/volumn2"
                    }
                },
                State {
                    name: "volumn3"
                    when: mainWindow.volumn>66
                    PropertyChanges {
                        target: speaker
                        source:"interfacepics/volumn3"
                    }
                }
            ]
            MouseArea{
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
                onClicked: {
                    if(mainWindow.volumn===0){
                        mainWindow.volumn=mainWindow.beforeMute
                    }
                    else{
                        mainWindow.beforeMute=mainWindow.volumn
                        mainWindow.volumn=0
                    }
                    mainWindow.volumnChange(volumnSlider.value)
                }
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
            value: mainWindow.volumn
            onMoved: {
                mainWindow.volumn=volumnSlider.value
                mainWindow.beforeMute=volumnSlider.value
                mainWindow.volumnChange(volumnSlider.value)
            }
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
                    source: "interfacepics/normalscreen"
                }
            },
            State {
                name: "normalScreen"
                PropertyChanges {
                    target: screenSize
                    source: "interfacepics/fullscreen"
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
            onClicked: {
                IF.screenSizeFunction()
            }
        }
    }
}
