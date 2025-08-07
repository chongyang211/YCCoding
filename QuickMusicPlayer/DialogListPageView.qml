import QtQuick

Item {
    Column {

        Text {
            text: "这个是弹窗，dialog，toast，pop等弹窗案例"
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
                text: "自定义列表弹窗"
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
                        palmModeDialog.open();
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
                text: "综合案例：联系人管理器"
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
                        stackView.push("CppQmlStudentPage.qml")
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


    // 当前选中的模式值
    property int currentPalmMode: 1

    // 模式选项数据
    readonly property var palmOptions: [
        { text: "识别模式", value: 1 },
        { text: "手机注册模式", value: 2 },
        { text: "设备注册模式", value: 3 },
        { text: "主机注册模式", value: 4 }
    ]

    // 自定义弹窗实例
    DialogCustomDialog {
        id: palmModeDialog
        // 弹窗打开时同步状态
        onDialogOpened: {
            console.log("Dialog opened! Syncing state...");
        }

        // 选项点击处理
        onItemClicked: function(selectedValue) {
            console.log("Selected option:", selectedValue);

            if (settingsPage.currentPalmMode === selectedValue) {
                console.log("No change in selection");
                return;
            }
            // 更新当前模式
            // 根据选择执行不同操作
            switch(selectedValue) {
            case 1:
                handleRecognitionMode();
                break;
            case 2:
                handlePhoneRegisterMode();
                break;
            case 3:
                handleDeviceRegisterMode();
                break;
            case 4:
                handleHostRegisterMode();
                break;
            }
        }
    }

}
