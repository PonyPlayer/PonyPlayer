import QtQuick

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
}
