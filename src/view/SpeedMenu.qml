import QtQuick 2.15
import QtQuick.Controls 2.15

Menu {
    Window{
        id:warning
        maximumHeight: 100
        maximumWidth: 200
        minimumHeight: 100
        minimumWidth: 200
        title: "ERROR"
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text:"纯音频无法开启八倍速"
        }
    }
    width:120
    title: "播放倍速"
    MenuItem{
        text: (mainWindow.speed==0.5?'✔':'    ')+"0.5倍速"
        onTriggered: {
            mainWindow.speed=0.5
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==1.0?'✔':'    ')+" 1.0倍速"
        onTriggered: {
            mainWindow.speed=1.0
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==1.25?'✔':'    ')+" 1.25倍速"
        onTriggered: {
            mainWindow.speed=1.25
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==1.5?'✔':'    ')+" 1.5倍速"
        onTriggered: {
            mainWindow.speed=1.5
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==2.0?'✔':'    ')+" 2.0倍速"
        onTriggered: {
            mainWindow.speed=2.0
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==4.0?'✔':'    ')+" 4.0倍速"
        onTriggered: {
            mainWindow.speed=4.0
            videoArea.setSpeed(mainWindow.speed)
        }
    }
    MenuItem{
        text: (mainWindow.speed==8.0?'✔':'    ')+" 8.0倍速"
        onTriggered: {
            if(!videoArea.hasVideo()){
                warning.show()
            }
            else{
                mainWindow.speed=8.0
                videoArea.setSpeed(mainWindow.speed)
            }
        }
    }
}

