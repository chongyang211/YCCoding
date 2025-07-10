import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    background: Rectangle {
        color: "#00AAAA"
    }

    width: parent.width
    Layout.fillWidth: true

    RowLayout{
        anchors.fill: parent

        ToolButton{
            icon.source: "qrc:/images/music.png"
            width: 32
            height: 32
            text: "关于"
        }
        ToolButton{
            icon.source: "qrc:/images/about.png"
            width: 32
            height: 32
            text: "杨充博客"
        }
        ToolButton{
            id:smallWindow
            icon.source: "qrc:/images/small-window.png"
            width: 32
            height: 32
            text: "小窗播放"
        }
        ToolButton{
            id:normalWindow
            icon.source: "qrc:/images/exit-small-window.png"
            width: 32
            height: 32
            text: "退出小窗播放"
            visible: false
        }
        Item{
            Layout.fillWidth: true
            height: 32
            Text {
                anchors.centerIn: parent
                text: "杨充"
                font.pointSize: 15
                color:"#ffffff"
            }
        }
        ToolButton{
            icon.source: "qrc:/images/minimize-screen.png"
            width: 32
            height: 32
            text: "最小化"
        }
        ToolButton{
            id:resize
            icon.source: "qrc:/images/small-screen.png"
            width: 32
            height: 32
            text: "退出全屏"
            visible: false
        }
        ToolButton{
            id:maxWindow
            icon.source: "qrc:/images/full-screen.png"
            width: 32
            height: 32
            text: "全屏"
        }
        ToolButton{
            icon.source: "qrc:/images/power.png"
            width: 32
            height: 32
            text: "退出"
        }
    }
}
