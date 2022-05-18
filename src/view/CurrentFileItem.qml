import QtQuick
import QtQuick.Controls

MenuItem {
    id:currentMenuItem
    property string filePath
    signal addFilePath(string path)
    onTriggered: {currentMenuItem.addFilePath(menuItem.filePath)
    }
}
