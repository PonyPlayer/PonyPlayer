import QtQuick
import QtQuick.Controls

MenuItem {
    id:currentMenuItem
    property string filePath
    property string fileName
    signal addFilePath(string path, string name)
    onTriggered: {
        currentMenuItem.addFilePath(currentMenuItem.filePath, currentMenuItem.fileName)
    }
}
