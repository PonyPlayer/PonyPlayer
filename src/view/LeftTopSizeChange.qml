import QtQuick
import QtQuick.Controls

Rectangle {
    id:leftTopSizeChange
    height: 1
    width: 2
    color: "#666666"
    MouseArea{
        anchors.fill:parent
        cursorShape:Qt.SizeFDiagCursor
        onPressed: {
            mainWindow.startSystemResize(Qt.LeftEdge | Qt.TopEdge);
        }
    }
}
