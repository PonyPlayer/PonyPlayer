import QtQuick 2.15
import QtQuick.Controls 2.15

MenuItem {
    id:device
    property string deviceName
    signal selectDevice(string d)
    onTriggered: {
        device.selectDevice(device.deviceName)
    }
}
