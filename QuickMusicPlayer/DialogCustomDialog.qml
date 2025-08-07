import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// CustomDialog.qml

Dialog {
    id: root

    // 公共属性
    property string dialogTitle: ""                      // 弹窗标题
    property var optionsModel: []                        // 选项数据模型
    property var currentValue                            // 当前选中的值
    property var onItemClicked: (function(option){})    // 点击选项时的回调
    property var onDialogOpened: (function(){})          // 弹窗打开时的回调

    width: parent.width * 0.9
    height: Math.min(parent.height * 0.8, contentColumn.implicitHeight + header.implicitHeight + Dimens.dp(40))
    modal: true
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    // 弹窗打开时触发回调
    onOpened: onDialogOpened()

    background: Rectangle {
        color: "#F6F8FB"
        radius: Dimens.dp(8)
    }

    header: Rectangle {
        width: parent.width
        implicitHeight: Dimens.dp(60)
        color: "white"
        radius: Dimens.dp(8)

        Text {
            text: dialogTitle
            color: "#333333"
            font.pixelSize: Dimens.dp(18)
            font.bold: true
            anchors.centerIn: parent
        }
    }

    contentItem: Column {
        id: contentColumn
        width: parent.width - Dimens.dp(32)
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: Dimens.dp(10)
        topPadding: Dimens.dp(10)
        bottomPadding: Dimens.dp(10)

        // 使用Repeater动态生成选项
        Repeater {
            model: optionsModel

            Rectangle {
                width: contentColumn.width
                height: Dimens.dp(60)
                radius: Dimens.dp(8)

                property bool isSelected: currentValue === modelData.value
                color: isSelected ? "#E0F7FA" : "transparent"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: Dimens.dp(10)
                    spacing: Dimens.dp(10)

                    Text {
                        text: modelData.text
                        font.pixelSize: Dimens.dp(16)
                        color: isSelected ? "#4CAF50" : "#333333"
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        width: Dimens.dp(20)
                        height: Dimens.dp(20)
                        radius: Dimens.dp(10)
                        color: isSelected ? "#4CAF50" : "#E0E0E0"
                        visible: isSelected

                        Text {
                            text: "✓"
                            color: "white"
                            font.pixelSize: Dimens.dp(14)
                            anchors.centerIn: parent
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // 触发点击回调并关闭弹窗
                        root.itemClicked(modelData.value)
                        root.close()
                    }
                }
            }
        }
    }

    // 触发点击回调 (保护性调用)
    function itemClicked(value) {
        if(typeof onItemClicked === "function") {
            onItemClicked(value);
        }
    }
}
