import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id:body
    //左侧播放列表栏
    Rectangle{
        id:videoList
        width: mainWindow.isVideoListOpen?200:0
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color:"black"
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
            }
            //启动打开文件
            Image{
                id:openFile
                width: 25
                height: 20
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: minimize.verticalCenter
                source: "PonyPics/FileOpener"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: "PointingHandCursor"
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }
            //关闭文件
//            Image {
//                id:removeFile
//                width: 25
//                height: 20
//                anchors.left: openFile.right
//                anchors.leftMargin: 5
//                anchors.verticalCenter: minimize.verticalCenter
//                source: "PonyPics/FileCloser"
//                MouseArea{
//                    anchors.fill: parent
//                    cursorShape: "PointingHandCursor"
//                    onClicked: {
//                    }
//                }
//            }
            //关闭播放栏列表
            Image {
                id: minimize
                source: "PonyPics/Minimize"
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
        ScrollView{
            id:videoScroll
            anchors.top: videoListOperator.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ListView {
                id:lllls
                 model: 20
//                     ListModel{
//                     {name:1},{name:1},{name:1},{name:1},{name:1},{name:1},{name:1},{name:1},{name:1},{name:1}
//                 }

                 delegate: Text {
                     width: parent.width
                     color: "white"
                     text:index
                     height: 20
                     MouseArea{
                         anchors.fill: parent
                         cursorShape: "PointingHandCursor"
                         onClicked: {
                             console.log(index)
                         }
                     }

                     required property int index
                 }
             }
        }


    }
    //视频播放区域
    SwipeView{
        id:mainArea
        orientation: Qt.Vertical
        anchors.left: videoList.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Rectangle{
            id:videoArea
            color:"gray"
            MouseArea{
                anchors.fill: parent
                propagateComposedEvents: true
                cursorShape: "PointingHandCursor"
                onClicked: {
                    if(mainWindow.isPlay){
                        mainWindow.isPlay=false
                    }
                    else{
                        mainWindow.isPlay=true
                    }
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
