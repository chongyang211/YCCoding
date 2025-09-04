import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {

    // 背景渐变色
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2c3e50" }
            GradientStop { position: 1.0; color: "#1a2530" }
        }
    }

    // 欢迎卡片
    Rectangle {
        id: welcomeCard
        width: 320
        height: 480
        anchors.centerIn: parent
        radius: 24
        color: "#ffffff"

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

                        transform: Scale {
                            id: resultScale
                            origin.x: resultImage.width / 2
                            origin.y: resultImage.height / 2
                            xScale: 0.6
                            yScale: 0.6
                        }
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

        // 底部标识
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 28
            text: "Palm"
            font.pixelSize: 16
            color: "#95a5a6"
        }

        // 初始状态隐藏
        opacity: 0
        scale: 0.9
        y: 40

        // 组件加载完成后执行进场动画
        Component.onCompleted: {
            countdownTimer.start();
               enterAnim.start();
        }
    }

    ParallelAnimation {
        id: enterAnim
        NumberAnimation { target: welcomeCard; property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutQuad }
        NumberAnimation { target: welcomeCard; property: "y"; from: Dimens.dp(20); to: 0; duration: 200; easing.type: Easing.OutQuad }
        NumberAnimation { target: resultScale; property: "xScale"; from: 0.6; to: 1.0; duration: 300; easing.type: Easing.OutBack }
        NumberAnimation { target: resultScale; property: "yScale"; from: 0.6; to: 1.0; duration: 300; easing.type: Easing.OutBack }
        SequentialAnimation {
            PauseAnimation { duration: 80 }
            NumberAnimation { target: subtitleText; property: "opacity"; from: 0.0; to: 1.0; duration: 140; easing.type: Easing.OutQuad }
        }
    }

    // 倒计时计时器
    Timer {
        id: countdownTimer
        interval: 1000
        repeat: true

        property int remaining: 3

        onTriggered: {
            remaining--
            countdownText.text = remaining + "s"
            if(remaining == 0) {
                countdownTimer.stop();
            }
        }
    }



}
