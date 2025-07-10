import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: window
    width: 1200
    height: 800
    visible: true
    title: qsTr("Cloud Music Player")

    ColumnLayout{
        anchors.fill: parent
        spacing: 0


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

        Frame{
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            background: Rectangle{
                color: "#f0f0f0"
            }
            padding: 0


        }

        //底部工具栏
        Rectangle{
            Layout.fillWidth: true
            height: 60
            color: "#00AAAA"

            Row{

                spacing: 10

                Image {
                    id: muisc
                    source: "qrc:/images/power.png"
                }
            }
        }
    }


    // 页面监听
    Component.onCompleted: {
        console.log("Resource path:", "qrc:/images/music.png")

    }

}
