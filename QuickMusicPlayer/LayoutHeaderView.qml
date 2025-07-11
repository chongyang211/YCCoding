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

        //ToolButton
        ToolButton{
            // icon: "qrc:/images/music.png"
            icon.source: "qrc:/images/music.png"
            width: 32
            height: 32
            text: "关于"
        }
        MusicToolButton{
            icon.source: "qrc:/images/about.png"
            width: 32
            height: 32
            text: "杨充博客"
            onClicked: {
                Qt.openUrlExternally("https://github.com/yangchong211")
            }
        }
        MusicToolButton{
            id:smallWindow
            icon.source: "qrc:/images/small-window.png"
            width: 32
            height: 32
            text: "小窗播放"
            onClicked: {
                setWindowSize(330,650)
                smallWindow.visible=false
                normalWindow.visible=true
            }
        }
        MusicToolButton{
            id:normalWindow
            icon.source: "qrc:/images/exit-small-window.png"
            width: 32
            height: 32
            text: "退出小窗播放"
            visible: false
            onClicked: {
                setWindowSize(window.mWINDOW_WIDTH,window.mWINDOW_HEIGHT)
                smallWindow.visible=true
                normalWindow.visible=false
            }
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
        MusicToolButton{
            icon.source: "qrc:/images/minimize-screen.png"
            width: 32
            height: 32
            text: "最小化"
            onClicked: {
                window.hide()
            }
        }
        MusicToolButton{
            id:resize
            icon.source: "qrc:/images/small-screen.png"
            width: 32
            height: 32
            text: "退出全屏"
            visible: false
            onClicked: {
                setWindowSize()
                window.visibility = Window.AutomaticVisibility
                maxWindow.visible = true
                resize.visible = false
            }
        }
        MusicToolButton{
            id:maxWindow
            icon.source: "qrc:/images/full-screen.png"
            width: 32
            height: 32
            text: "全屏"
            onClicked: {
                window.visibility = Window.Maximized
                maxWindow.visible = false
                resize.visible = true
            }
        }
        ToolButton{
            icon.source: "qrc:/images/power.png"
            width: 32
            height: 32
            text: "退出"
            onClicked: {
                Qt.quit()
            }
        }
    }

    function setWindowSize(width = window.mWINDOW_WIDTH,height = window.mWINDOW_HEIGHT) {
        //这个是宽高
        window.width = width
        window.height = height
        //这个是坐标
        window.x=(Screen.desktopAvailableWidth-window.width)/2
        window.y=(Screen.desktopAvailableHeight-window.height)/2
    }
}
