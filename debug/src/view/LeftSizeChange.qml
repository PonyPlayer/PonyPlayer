import QtQuick
import QtQuick.Controls

Rectangle {
    id:leftSizeChange;
    width: 2
    color: "#666666"
    MouseArea{
        anchors.fill:parent
        cursorShape:Qt.SizeHorCursor
        onPressed: {
            mainWindow.startSystemResize(Qt.LeftEdge);
        }
    }
}
