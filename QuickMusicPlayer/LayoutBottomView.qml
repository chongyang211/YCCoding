import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts



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

            //滑块进度条
            Slider{
                id:slider
                width: parent.width
                Layout.fillWidth: true
                height: 25
                background:Rectangle{
                    x:slider.leftPadding
                    y:slider.topPadding+(slider.availableHeight-height)/2
                    width: slider.availableWidth
                    height: 4
                    radius: 2
                    color: "#e9f4ff"
                    Rectangle{
                        width: slider.visualPosition*parent.width
                        height: parent.height
                        color: "#73a7ab"
                        radius: 2
                    }
                }
                handle:Rectangle{
                    x:slider.leftPadding+(slider.availableWidth-width)*slider.visualPosition
                    y:slider.topPadding+(slider.availableHeight-height)/2
                    width: 15
                    height: 15
                    radius: 5
                    color: "#f0f0f0"
                    border.color: "#73a7ab"
                    border.width: 0.5
                }
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
