import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import PonyPlayer
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
    property int speed: 1
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
    signal play()
    //暂停
    signal pause()
    //下一个
    signal nextOne()
    //上一个
    signal lastOne()
    //倒放
    signal inverted()
    //音量改变
    signal volumnChange(int vol)
    //播放进度改变
    signal currentTimeChange(int cur)
    //播放模式改变
    signal playModeChange(string state)
    //播放倍速改变
    signal speedChange(int speed)
    //打开文件
    signal openFilePath(string path)




    width: mainWindow.userWidth
    height: mainWindow.userHeight
    minimumWidth: 650
    minimumHeight: 500
    visible: true
    title: "PonyPlayer"
    Component.onCompleted: {
        mainWindow.openFilePath.connect(IF.mytest)
    }

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
        Component.onCompleted: {
            mainWindow.currentTimeChange.connect(femo)
        }
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
