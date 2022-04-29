import QtQuick
import QtQuick.Controls

Window {
    id: mediainfowindow
    width: 300
    height: 600

    // 列表代理
    Component {
        id: mediaInfoListDelegate
        Item {
            id: mediaInfoListitem

            height: 20
            width: parent.width

            Row {
                id: raylay
                anchors.fill:parent
                anchors.verticalCenter: parent.verticalCenter
                padding: 2
                spacing: 2


                Text {
                    text: info_key
                    font.bold: true
                    font.pointSize: 18

                }

                Text {
                    text: info_value
                    font.bold: false
                    font.pointSize: 18
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
                 ListElement{
                     info_key:"7"
                     info_value: 7
                 }
                 ListElement{
                     info_key:"5"
                     info_value: 6
                 }
                 ListElement{
                     info_key:"3"
                     info_value: 9
                 }
                 ListElement{
                     name:"1"
                     age: 45
                 }
             }

            delegate: mediaInfoListDelegate
         }
    }
}

