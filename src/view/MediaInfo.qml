import QtQuick
import QtQuick.Controls
import Controller

Window {
    id: infwin
    width: 300
    height: 600
    property string filepath
    property alias infomodel: medialistModel

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

            height: 20
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
                    font.bold: true
                    font.pointSize: 18
                }

                Text {
                    text: infocontent
                    elide: Text.ElideMiddle
                    font.pointSize: 18
                    width: parent.width-info_key.contentWidth
                }

            }

        }

    }


    ScrollView{
        id: mediainfolist
        anchors.fill: parent

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
