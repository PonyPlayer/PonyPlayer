import QtQuick
import QtQuick.Controls

MenuItem {
    id:menuItem
    property int trackID
    signal setTrack(int i)
    onTriggered: menuItem.setTrack(menuItem.trackID)
}
