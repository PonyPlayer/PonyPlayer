import QtQuick
import QtQuick.Controls

Rectangle {
    id:downSizeChange;
    height: 1;
    color: "#666666"
    MouseArea{
        anchors.fill:parent
        cursorShape:Qt.SizeVerCursor
        onPressed: {
            mainWindow.startSystemResize(Qt.BottomEdge);
        }
    }
}
