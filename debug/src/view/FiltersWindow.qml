import QtQuick
import QtQuick.Controls
import "./interfacefunctions.js" as IF
Window {
    id:filterswindow
    minimumWidth: 820
    minimumHeight: 540
    maximumWidth: 820
    maximumHeight: 540
    ListModel{
        id:filtermodel
    }

    ScrollView{
        width: parent.width
        height: parent.height
        Grid{
            columns: 4
            Repeater{
                model: filtermodel
                delegate:FilterItem{
                    filterName:filterNames
                    image:images
                    lut:luts
                }
            }
        }
        Component.onCompleted: {
            IF.loadingFilters()
        }
    }
}