import QtQuick
import QtQuick.Controls

Rectangle {
    id:rightTopSizeChange
    height: 1
    width: 2
    color: "#666666"
    MouseArea{
        anchors.fill:parent
        cursorShape:Qt.SizeBDiagCursor
        onPressed: {
            mainWindow.startSystemResize(Qt.RightEdge | Qt.TopEdge);
        }
    }
}
