import QtQuick
import QtQuick.Controls
import WaveView

Window {
    id: wavewindow
    width: 600
    height: 600
    Image {
            id: background
            anchors.fill: parent
            source: "img/pexels-flora-westbrook-2574614"
    }
    WaveView{
        id: wavearea
        radiusRatio: 1
        anchors.fill: parent
        Behavior on radiusRatio{
            NumberAnimation {
                duration: 200
                easing.type: Easing.InQuad
            }
        }

        Rectangle {
            id: disc_border
            anchors.centerIn: parent
            property double radiusRatio;
            width: Math.min(parent.width,parent.height)/(2+(0.5)*(1-radiusRatio))
            height: width
            radius: width*0.5
            opacity: 0.3
            RoundImage {
                id: disc
                anchors.centerIn: parent
                width: Math.min(parent.parent.width,parent.parent.height)/3
                height: width
                source: "img/pexels-izabella-arvai-953475"
                edge: 0.95
                RotationAnimation on rotation {
                    loops: Animation.Infinite
                    from: 0
                    to: 360
                    duration: 15000
                }
            }
        }
        Binding{
            target: disc_border;
            property: "radiusRatio";
            value: wavearea.radiusRatio;
        }
    }

}

