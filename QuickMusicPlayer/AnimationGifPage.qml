import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {

    // 页面状态管理
    property string currentPage: "home"

    // 背景
    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"
    }

    // 主页内容
    Page {
        id: homePage
        anchors.fill: parent
        visible: currentPage === "home"

        background: Rectangle {
            color: "#f5f5f5"
        }

        Column {
            anchors.centerIn: parent
            spacing: 30

            Text {
                text: "主页"
                font.pixelSize: 32
                font.bold: true
                color: "#333333"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: "进入欢迎页面"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    currentPage = "transition"
                    transitionAnimation.start()
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

    // 过渡动画层
    Item {
        id: transitionLayer
        anchors.fill: parent
        visible: currentPage === "transition"

        // 半透明背景
        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.7
        }

        // GIF 动画
        AnimatedImage {
            id: transitionGif
            anchors.centerIn: parent
            width: 200
            height: 200
            source: "qrc:/icons/images/palm.gif" // 替换为您的GIF路径
            playing: true

            // 动画状态变化处理
            onStatusChanged: {
                if (status === AnimatedImage.Ready) {
                    console.log("GIF加载完成")
                } else if (status === AnimatedImage.Error) {
                    console.log("GIF加载错误")
                    // 出错时直接跳转到欢迎页面
                    currentPage = "welcome"
                }
            }

            // 动画播放完成处理
            onCurrentFrameChanged: {
                if (currentFrame === frameCount - 1) {
                    // GIF播放完成后跳转到欢迎页面
                    currentPage = "welcome"
                }
            }
        }

        // 加载指示器（GIF加载期间显示）
        BusyIndicator {
            anchors.centerIn: parent
            running: transitionGif.status === AnimatedImage.Loading
            visible: running
        }
    }

    // 欢迎页面（基于您提供的图片描述）
    Page {
        id: welcomePage
        anchors.fill: parent
        visible: currentPage === "welcome"

        background: Rectangle {
            color: "white"
        }

        // 顶部状态栏
        Row {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            spacing: 10

            // 关闭按钮和倒计时
            Button {
                id: closeButton
                text: "× 3s"
                width: 60
                height: 30
                font.pixelSize: 16

                background: Rectangle {
                    color: "transparent"
                }

                contentItem: Text {
                    text: parent.text
                    color: "black"
                    font: parent.font
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    currentPage = "home"
                }
            }

            Item { width: parent.width - 180; height: 1 } // 占位

            // WiFi图标
            Text {
                text: "📶" // WiFi图标
                font.pixelSize: 20
                color: "black"
            }
        }

        // 主要内容区域
        Column {
            anchors.centerIn: parent
            spacing: 20

            // 绿色对勾图标（使用AnimatedImage替代静态图片）
            AnimatedImage {
                id: checkmarkGif
                width: 100
                height: 100
                source: "qrc:/icons/images/palm.gif" // 替换为您的GIF路径
                playing: true
                anchors.horizontalCenter: parent.horizontalCenter

                // 动画播放完成后保持最后一帧
                onCurrentFrameChanged: {
                    if (currentFrame === frameCount - 1) {
                        playing = false
                    }
                }
            }

            // 欢迎文本
            Text {
                text: "Welcome"
                font.pixelSize: 28
                font.bold: true
                color: "black"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "XiJie Yuan"
                font.pixelSize: 22
                color: "black"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Verification time: 2024-11-11 14:53:04"
                font.pixelSize: 14
                color: "gray"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        // 底部标识
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 30
            text: "Palm"
            font.pixelSize: 16
            color: "gray"
        }

        // 3秒倒计时自动关闭
        Timer {
            id: countdownTimer
            interval: 1000
            repeat: true

            property int remaining: 3

            onTriggered: {
                remaining--
                closeButton.text = "× " + remaining + "s"

                if (remaining <= 0) {
                    currentPage = "home"
                    stop()
                }
            }
        }

        // 页面显示时启动倒计时
        onVisibleChanged: {
            if (visible) {
                closeButton.text = "× 3s"
                remaining = 3
                countdownTimer.start()
            } else {
                countdownTimer.stop()
            }
        }
    }

    // 过渡动画序列
    SequentialAnimation {
        id: transitionAnimation

        // 等待GIF动画播放
        PauseAnimation {
            duration: 2000 // 假设GIF时长为2秒
        }

        // 切换到欢迎页面
        ScriptAction {
            script: {
                currentPage = "welcome"
            }
        }
    }

    // 页面切换动画
    states: [
        State {
            name: "home"
            PropertyChanges { target: homePage; opacity: 1 }
            PropertyChanges { target: welcomePage; opacity: 0 }
        },
        State {
            name: "welcome"
            PropertyChanges { target: homePage; opacity: 0 }
            PropertyChanges { target: welcomePage; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "home"
            to: "welcome"

            SequentialAnimation {
                // 主页淡出
                NumberAnimation {
                    target: homePage
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 500
                }

                // 欢迎页淡入
                NumberAnimation {
                    target: welcomePage
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 500
                }
            }
        },

        Transition {
            from: "welcome"
            to: "home"

            SequentialAnimation {
                // 欢迎页淡出
                NumberAnimation {
                    target: welcomePage
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 500
                }

                // 主页淡入
                NumberAnimation {
                    target: homePage
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 500
                }
            }
        }
    ]

}
