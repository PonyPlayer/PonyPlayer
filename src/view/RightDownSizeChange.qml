import QtQuick
import QtQuick.Controls.Material
Rectangle {//向左下拖动
    id:rightDownSizeChange
    height: 4;//远离发送的按钮
    width: 4
    color: "#666666"
    MouseArea{  //设置可以拖动没有标题的登录界面.  /*这个要放在上面，放在最下面的话，会把上面全部屏蔽掉的*/
        anchors.fill:parent
        property point clickPos: "0,0"  //定义一个点
        cursorShape:Qt.SizeFDiagCursor
        onPressed: {
            clickPos = Qt.point(mouseX, mouseY)
        }

        onPositionChanged: {  //属性的改变
            var delta = Qt.point(mouseX-clickPos.x, mouseY-clickPos.y)
            if((mainWindow.height+delta.y)>mainWindow.minimumHeight)
                mainWindow.height += delta.y
            else
                mainWindow.setHeight(mainWindow.minimumHeight)
            if((mainWindow.width+delta.x)>mainWindow.minimumWidth)
            {
               mainWindow.setWidth(mainWindow.width+delta.x)
            }
            else
                mainWindow.setWidth(mainWindow.minimumWidth)
        }
    }
}
