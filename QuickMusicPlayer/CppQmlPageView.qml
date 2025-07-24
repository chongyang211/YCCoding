import QtQuick

Item {
    Column {
        Text {
            text: "C++和QML交互，综合案例"
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

        Row {
            spacing: 10
            anchors.topMargin: 30

            Text {
                text: "信号槽1-> C++到QML通信"
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
                text: "信号槽2-> QML到C++通信"
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
                        stackView.push("CppQmlSignls2Page.qml")
                    }
                }
            }


            Text {
                text: "用户登录功能"
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
                        stackView.push("CppQmlLogginPage.qml")
                    }
                }
            }

            Text {
                text: "数据列表展示"
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
                        stackView.push("CppQmlListPage.qml")
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
