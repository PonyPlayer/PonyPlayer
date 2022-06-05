import QtQuick
import QtQuick.Controls

Rectangle {
    id:leftDownSizeChange
    height: 1
    width: 2
    color: "#666666"
    MouseArea{
        anchors.fill:parent
        cursorShape:Qt.SizeBDiagCursor
        onPressed: {
            mainWindow.startSystemResize(Qt.LeftEdge | Qt.BottomEdge);
        }
    }
}
