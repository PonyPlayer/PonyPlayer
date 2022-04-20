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
                onAccepted: {
                    mainWindow.openFilePath(fileDialog.currentFile);
                }
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
                source: "interfacepics/fileopener"
                Shortcut{
                    sequence: "Ctrl+I"
                    onActivated: fileDialog.open();
                }
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
                source: "interfacepics/minimize"
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
                delegate: Rectangle {
                     color: "transparent"
                     Text {
                         text: name + "  "+index
                         color: "white"
                         lineHeight: 20
                     }
                     width: 200
                     height: 24
                     MouseArea{
                         anchors.fill: parent
                         cursorShape: "PointingHandCursor"
                         onClicked: {
                             listview.currentIndex=index
                         }
                     }
                     Image{
                         height: 20
                         width: 20
                         source: "interfacepics/filecloser"
                         anchors.right: parent.right
                         anchors.top: parent.top
                         anchors.rightMargin: 2
                         anchors.topMargin: 2
                         MouseArea{
                             anchors.fill: parent
                             cursorShape: "PointingHandCursor"
                             onClicked: {
                                 listModel.remove(index,1)
                             }
                         }
                     }
                 }
             }
        }


    }
    //视频播放区域
    SwipeView{
        id:mainArea
        orientation: Qt.Horizontal
        anchors.left: videoList.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        clip:true
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

