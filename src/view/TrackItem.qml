import QtQuick
import QtQuick.Controls

MenuItem {
    id:menuItem
    property int trackID
    property string trackName
    signal setTrack(int i)
    text: (mainWindow.audioTrack==menuItem.trackName?'✔':'    ')+menuItem.trackName
    onTriggered: menuItem.setTrack(menuItem.trackID)
}
