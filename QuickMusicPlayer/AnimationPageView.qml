import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {

    Column {

        Text {
            text: "动画效果综合案例"
            font.pixelSize: 20
            color: "black"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter

            //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.color = "red"
                }
                onExited: {
                    parent.color = "black"
                }
            }
        }

        Item {
            height: 50
        }

        Row {
            spacing: 10


            Text {
                text: "使用StackView的过渡动画"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                        stackView.push("CppQmlSignls1Page.qml")
                    }
                }
            }

            Text {
                text: "使用状态（State）和过渡（Transition）"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                        stackView.push("AnimationStatesPage.qml")
                    }
                }
            }

            Text {
                text: "使用AnimatedImage播放gif动画"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                           // stackView.push("AnimationStatesPage.qml")
                        stackView.push("AnimationPageView.qml")
                    }
                }
            }
        }


        Item {
            height: 50
        }

        Row {
            spacing: 10

            Text {
                text: "PropertyAnimation控制页面属性动画1"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                        stackView.push("AnimationPropertyPage1.qml")
                    }
                }
            }

            Text {
                text: "PropertyAnimation控制页面属性动画2"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                        stackView.push("AnimationPropertyPage2.qml")
                    }
                }
            }

            Text {
                text: "实时数据更新"
                font.pixelSize: 20
                color: "black"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter

                //创建一个鼠标悬停效果，当鼠标悬停在文本上时，改变文本的颜色。
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.color = "red"
                    }
                    onExited: {
                        parent.color = "black"
                    }
                    onClicked: {
                        stackView.push("CppQmlDataChangePage.qml")
                    }
                }
            }
        }
    }

}
