import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {
    id: appWindow

    // 定义页面状态
        property string currentPage: "main"

        // 背景
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#1a5276" }
                GradientStop { position: 1.0; color: "#2874a6" }
            }
        }

        // 页面容器
        Item {
            id: pageContainer
            anchors.fill: parent

            // 主页面
            Page {
                id: mainPage
                anchors.fill: parent
                visible: currentPage === "main"

                background: Rectangle {
                    color: "transparent"
                }

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 20

                    Text {
                        text: "主页面"
                        font.pixelSize: 28
                        font.bold: true
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Button {
                        text: "跳转下一个页面"
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: {
                            appWindow.currentPage = "pass"
                        }

                        background: Rectangle {
                            color: "#4CAF50"
                            radius: 8
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 18
                        }
                    }
                }
            }

            // 欢迎卡片
            Page {
                id: welcomeCard
                width: 320
                height: 480
                anchors.centerIn: parent
                // 顶部状态栏
                Row {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 24
                    spacing: 8

                    // 倒计时按钮
                    Rectangle {
                        width: 80
                        height: 30
                        radius: 15
                        color: "#f2f4f7"

                        Text {
                            id: countdownText
                            text: "3s"
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            color: "#34495e"
                        }
                    }

                    Item { width: parent.width - 180; height: 1 } // 占位

                    // WiFi状态
                    Text {
                        text: "📶"
                        font.pixelSize: 20
                    }
                }

                // 主要内容
                Column {
                    id: welcomeColumn
                    anchors.centerIn: parent
                    spacing: 28
                    width: parent.width - 60

                    // 动画容器
                    Item {
                        width: 140
                        height: 140
                        anchors.horizontalCenter: parent.horizontalCenter

                        // 圆形背景
                        Rectangle {
                            id: circleBg
                            anchors.fill: parent
                            radius: width / 2
                            color: "#e8f5e9" // 浅绿色背景

                            // 勾动画
                            Image {
                                id: checkmark
                                source: "checkmark.png"
                                anchors.centerIn: parent
                                width: 80
                                height: 80
                                opacity: 0
                                scale: 0.8
                            }
                        }
                    }

                    // 文本区域
                    Column {
                        spacing: 12
                        width: parent.width

                        Text {
                            text: "Welcome"
                            font.pixelSize: 28
                            font.weight: Font.DemiBold
                            color: "#2c3e50"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            id: subtitleText
                            text: "XiJie Yuan"
                            font.pixelSize: 24
                            color: "#2c3e50"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: "Verification time: " + Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss")
                            font.pixelSize: 14
                            color: "#7f8c8d"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
        }

        // 定义页面状态
        states: [
            // 主页面状态
            State {
                name: "mainState"
                when: currentPage === "main"

                PropertyChanges {
                    target: mainPage
                    x: 0
                    opacity: 1
                }

                PropertyChanges {
                    target: welcomeCard
                    x: parent.width
                    opacity: 0
                }
            },

            // 密码页面状态
            State {
                name: "passState"
                when: currentPage === "pass"

                PropertyChanges {
                    target: mainPage
                    x: -parent.width
                    opacity: 0
                }

                PropertyChanges {
                    target: welcomeCard
                    x: 0
                    opacity: 1
                }
            }
        ]

        // 定义页面过渡动画
        transitions: [
            // 从主页面到密码页面的过渡
            Transition {
                from: "mainState"
                to: "passState"

                SequentialAnimation {
                    // 主页面向左滑出
                    ParallelAnimation {
                        NumberAnimation {
                            target: mainPage
                            property: "x"
                            from: 0
                            to: -parent.width
                            duration: 400
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            target: mainPage
                            property: "opacity"
                            from: 1
                            to: 0.5
                            duration: 300
                        }
                    }

                    // 密码页面从右侧滑入
                    ParallelAnimation {
                        NumberAnimation {
                            target: welcomeCard
                            property: "x"
                            from: appWindow.width
                            to: 0
                            duration: 500
                            easing.type: Easing.OutBack
                        }

                        NumberAnimation {
                            target: welcomeCard
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 500
                        }
                    }
                }
            },

            // 从密码页面回到主页面的过渡
            Transition {
                from: "passState"
                to: "mainState"

                SequentialAnimation {
                    // 密码页面向右滑出
                    ParallelAnimation {
                        NumberAnimation {
                            target: welcomeCard
                            property: "x"
                            from: 0
                            to: appWindow.width
                            duration: 400
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            target: welcomeCard
                            property: "opacity"
                            from: 1
                            to: 0.5
                            duration: 300
                        }
                    }

                    // 主页面从左侧滑入
                    ParallelAnimation {
                        NumberAnimation {
                            target: mainPage
                            property: "x"
                            from: -appWindow.width
                            to: 0
                            duration: 500
                            easing.type: Easing.OutBack
                        }

                        NumberAnimation {
                            target: mainPage
                            property: "opacity"
                            from: 0.5
                            to: 1
                            duration: 500
                        }
                    }
                }
            }
        ]

        SequentialAnimation {
            id: errorAnimation

            ParallelAnimation {
                NumberAnimation {
                    target: welcomeColumn
                    property: "x"
                    from: 0
                    to: -10
                    duration: 100
                }

                // ColorAnimation {
                //     target: welcomeColumn
                //     property: "border.color"
                //     from: "#bdc3c7"
                //     to: "#e74c3c"
                //     duration: 100
                // }
            }

            ParallelAnimation {
                NumberAnimation {
                    target: welcomeColumn
                    property: "x"
                    from: -10
                    to: 10
                    duration: 100
                }
            }

            ParallelAnimation {
                NumberAnimation {
                    target: welcomeColumn
                    property: "x"
                    from: 10
                    to: 0
                    duration: 100
                }

                // ColorAnimation {
                //     target: welcomeColumn
                //     property: "border.color"
                //     from: "#e74c3c"
                //     to: "#bdc3c7"
                //     duration: 300
                // }
            }
        }
    }
}
