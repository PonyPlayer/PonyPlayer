import QtQuick
import QtQuick.Controls
import WaveView
import LyricsData
import LyricSentence
import "./interfacefunctions.js" as IF

Rectangle {
    property alias waveArea: waveArea
    property alias lyricsArea: lyricsArea
    property alias lyricsData: lyricsData
    width: 600
    height: 600
    Image {
        id: background
        anchors.fill: parent
        source: "img/pexels-flora-westbrook-2574614"
    }
    LyricsData {
        id: lyricsData
    }
    WaveView {
        id: waveArea
        player: "videoArea"
        lyricsDataName: "lyricsData"
        radiusRatio: 1
        anchors.fill: parent
        Behavior on radiusRatio{
        NumberAnimation {
            duration: 200
            easing.type: Easing.InQuad
        }
    }

    Rectangle {
        id: discArea
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width: parent.width/2
        color: "transparent"
        Rectangle {
            id: disc_border
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            property double radiusRatio;
            width: Math.min(parent.width, parent.height)/(1.5+(0.5)*(1-radiusRatio))
            height: width
            radius: width*0.5
            opacity: 0.3
            RoundImage {
                id: disc
                anchors.centerIn: parent
                width: Math.min(parent.parent.width, parent.parent.height)/2
                height: width
                source: "img/pexels-izabella-arvai-953475"
                edge: 0.95
                RotationAnimation on rotation {
                loops: Animation.Infinite
                from: 0
                to: 360
                duration: 15000
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    IF.mytest(lyricsData.sentences[0].sentence);
                }
            }
        }
    }
}

Rectangle {
    id: lyricsArea
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: discArea.right
    height: parent.height*0.8
    width: parent.width/2*0.9
    color: "transparent"
    property alias flick: flick
    property alias rep: rep

    Flickable {
        property int currentIndex: 0;
        id: flick
        anchors.fill: lyricsArea
        contentWidth: width
        contentHeight: lyricsColumn.height
        contentY: 0

        clip: true
        Component {
            id: lycContent
            Rectangle {
                id: lycRec
                width: lyricsArea.width
                height: childText.paintedHeight
                color: "transparent"
                Text {
                    id: childText
                    width: lyricsArea.width
                    horizontalAlignment: Text.AlignHCenter
                    text: lyricsData.sentences[index].sentence
                        font.pointSize: 15
                        font.bold: flick.currentIndex==index ? true: false
                        color: "white"
                        opacity: flick.currentIndex==index ? 1: 0.7
                        wrapMode: Text.Wrap
                    }
                }
            }
            Column {
                id: lyricsColumn
                spacing: 30
                Repeater {
                    id: rep
                    model: lyricsData.sentences.length
                    delegate: lycContent
                }
            }
            Behavior on contentY {
            NumberAnimation {
                id: bouncebehavior
                easing {
                    type: Easing.OutQuad
                    amplitude: 1.0
                    period: 1.0
                }
            }
        }
    }
    Rectangle{
        anchors.fill: lyricsArea
        color: "transparent"
        Text {
            visible: lyricsData.sentences.length==0 ? true: false
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "无歌词"
            font.pointSize: 15
            font.bold: true
            color: "white"
            opacity: 1
            wrapMode: Text.Wrap
        }
    }
}

Binding{
    target: disc_border;
    property: "radiusRatio";
    value: waveArea.radiusRatio;
}
}

}

