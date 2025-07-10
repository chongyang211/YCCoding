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

            RowLayout {
                anchors.fill: parent

                Item{
                    Layout.preferredWidth: parent.width/10
                    Layout.fillWidth: true
                }
                Button{
                    icon.source: "qrc:/images/previous.png"
                    width: 32
                    height: 32
                    text: "上一曲"
                }

                Button{
                    icon.source: "qrc:/images/stop"
                    width: 32
                    height: 32
                    text: "暂停/播放"
                }
                Button{
                    icon.source: "qrc:/images/next.png"
                    width: 32
                    height: 32
                    text: "下一曲"
                }


                Item{
                    Layout.preferredWidth: parent.width/2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 25

                    Text{
                        id:nameText
                        anchors.left:slider.left
                        anchors.bottom: slider.top
                        anchors.leftMargin: 5
                        text:"杨充"
                        color: "#ffffff"
                    }
                    Text{
                        id:timeText
                        anchors.right: slider.right
                        anchors.bottom: slider.top
                        anchors.rightMargin: 5
                        text:"00:00/05:30"
                        color: "#ffffff"
                    }

                    Slider{
                        id:slider
                        from: 0 // 设置最小值
                        to: 100 // 设置最大值
                        width: parent.width
                        Layout.fillWidth: true
                        height: 25
                    }
                }

                Button{
                    Layout.preferredWidth: 100
                    icon.source: "qrc:/images/favorite"
                    width: 32
                    height: 32
                    text: "我喜欢"
                }
                Button{
                    Layout.preferredWidth: 100
                    icon.source: "qrc:/images/repeat"
                    width: 32
                    height: 32
                    text: "重复播放"
                }
                Item{
                    Layout.preferredWidth: parent.width/10
                    Layout.fillWidth: true
                }
            }
        }
    }


    // 页面监听
    Component.onCompleted: {
        console.log("Resource path:", "qrc:/images/music.png")

    }

}
