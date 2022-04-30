import QtQuick
import QtQuick.Controls
import WaveView

Window {
    id: wavewindow
    width: 600
    height: 600
    WaveView{
        id: wavearea
        anchors.fill: parent
    }
}

