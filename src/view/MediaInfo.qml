import QtQuick
import QtQuick.Controls.Material
import Controller

Window {
    id: infwin
    minimumHeight: 360
    minimumWidth: 360
    width: 360
    height: 360
    property string filepath
    property alias infomodel: medialistModel
    Rectangle{
        color:"#666666"
        anchors.fill:parent
    }
    Rectangle {
        id:wintitle
        width: parent.width
        height: 40
        Text {
            text:"媒体简介"
            anchors.centerIn: parent
            font.bold: true
            font.pixelSize: 30
        }
    }



//    Controller {
//            id: mediaInfoController

//            onFinishGetInfo: {
//                var infoitem = mediaLibController.getListItemInfo()
////                        medialistModel.append(infoitem)
//                console.log(infoitem["音频流大小"])
//                for(var infokey in infoitem) {
//                    medialistModel.append({"infokey":infokey, "infocontent":infoitem[infokey]})
//                }

////                        for(var i=0;i<items.length;i++) {
////                            console.log(items[i]["fileName"])
////                            // listModel.append({"name":items[i].getFileName,"file_path":items[i].getFilePath,"iconpath":"interfacepics/defaultlogo"})
////                            listModel.append(items[i])
////                            var appe = listModel.get(listModel.count-1)
////                            if(appe.iconPath == "")
////                                appe.iconPath = "interfacepics/defaultlogo"
////                        }
//            }
//        }

    // 列表代理
    Component {
        id: mediaInfoListDelegate
        Item {
            id: mediaInfoListitem

            height: 30
            width: infwin.width

            Row {
                id: raylay
                anchors.fill:parent
                anchors.verticalCenter: parent.verticalCenter
                padding: 2
                spacing: 2

                Text {
                    id: info_key
                    text: infokey
                    color: "pink"
                    font.bold: true
                    width:200
                    font.pointSize: 18
                }


                Text {
                    text: infocontent
                    elide: Text.ElideMiddle
                    color:"white"
                    font.pointSize: 18
                    width: parent.width-info_key.contentWidth

                }





            }

        }

    }


    ScrollView{
        id: mediainfolist
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom: parent.bottom
        anchors.top: wintitle.bottom

        ListView {
            anchors.fill: parent
            id:meidalistview
            focus:true
            model: ListModel{
                id:medialistModel
             }

            delegate: mediaInfoListDelegate
         }
    }

    // 调用 Controller 槽函数获取信息
//    Component.onCompleted: {
//        console.log("filepath:",mediainfowindow.filepath)
//        mediaLibController.sendGetInfoRequirement(mediainfowindow.filepath)
//    }

}
