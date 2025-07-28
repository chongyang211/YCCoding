import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: editorDialog
    width: Math.min(parent.width * 0.9, 500)
    height: Math.min(parent.height * 0.9, 700)
    radius: 12
    color: colors.cardBackground
    border.width: 1
    border.color: colors.highlight

    // 属性：当前编辑的联系人索引，-1表示新增
    property int contactIndex: -1

    // 属性：用于从外部传入联系人数据
    property var contactData: contactIndex !== -1 ? contactModel.getContactDetails(contactIndex) : ({})

    // 关闭请求信号
    signal closeRequested()

    // 保存成功信号
    signal saveCompleted(int index)

    // 关闭按钮
    ToolButton {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        icon.source: "images/close.png"
        icon.color: colors.text
        onClicked: closeRequested()
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: editorForm.width
        contentHeight: editorForm.height
        clip: true

        ColumnLayout {
            id: editorForm
            width: editorDialog.width - 40
            anchors.margins: 20
            anchors.top: parent.top
            spacing: 15

            Label {
                text: contactIndex === -1 ? "添加新联系人" : "编辑联系人"
                font.bold: true
                font.pointSize: 18
                Layout.alignment: Qt.AlignHCenter
                color: colors.text
            }

            // 头像选择区域
            Item {
                Layout.alignment: Qt.AlignHCenter
                width: 120
                height: 120

                Rectangle {
                    id: avatarPreview
                    anchors.fill: parent
                    radius: width / 2
                    color: "transparent"
                    border.width: 2
                    border.color: colors.accent

                    Image {
                        id: avatarImage
                        anchors.fill: parent
                        anchors.margins: 4
                        source: contactData.avatar || ""
                        fillMode: Image.PreserveAspectCrop
                        layer.enabled: true
                    }

                    // 默认头像占位符
                    Rectangle {
                        visible: !avatarImage.source || avatarImage.status !== Image.Ready
                        anchors.fill: parent
                        anchors.margins: 4
                        radius: width / 2
                        color: colors.accent
                        opacity: 0.2

                        Label {
                            anchors.centerIn: parent
                            text: contactData.name ? contactData.name.charAt(0) : "+"
                            font.pointSize: 36
                            color: colors.text
                        }
                    }
                }

                // 头像操作按钮
                RowLayout {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    ToolButton {
                        icon.source: "images/camera.png"
                        icon.color: colors.text
                        ToolTip.text: "拍照"
                        ToolTip.visible: hovered
                        onClicked: contactModel.showMessage("拍照功能")
                    }

                    ToolButton {
                        icon.source: "images/gallery.png"
                        icon.color: colors.text
                        ToolTip.text: "从相册选择"
                        ToolTip.visible: hovered
                        onClicked: contactModel.showMessage("相册选择")
                    }

                    ToolButton {
                        icon.source: "images/delete.png"
                        icon.color: "red"
                        visible: contactData.avatar && contactData.avatar.toString() !== ""
                        ToolTip.text: "移除头像"
                        ToolTip.visible: hovered
                        onClicked: avatarImage.source = ""
                    }
                }
            }

            // 表单字段
            GridLayout {
                columns: 2
                columnSpacing: 15
                rowSpacing: 10
                Layout.fillWidth: true

                // 姓名
                Label {
                    text: "姓名"
                    Layout.row: 0
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                TextField {
                    id: nameField
                    Layout.row: 0
                    Layout.column: 1
                    Layout.fillWidth: true
                    placeholderText: "必填"
                    text: contactData.name || ""
                    color: colors.text
                    onTextChanged: validateForm()
                }

                // 电话
                Label {
                    text: "电话"
                    Layout.row: 1
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                TextField {
                    id: phoneField
                    Layout.row: 1
                    Layout.column: 1
                    Layout.fillWidth: true
                    placeholderText: "必填"
                    text: contactData.phone || ""
                    color: colors.text
                    inputMethodHints: Qt.ImhDialableCharactersOnly
                    validator: RegularExpressionValidator {
                        regularExpression: /^[0-9\+\-\s]{7,20}$/
                    }
                    onTextChanged: validateForm()
                }

                // 邮箱
                Label {
                    text: "邮箱"
                    Layout.row: 2
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                TextField {
                    id: emailField
                    Layout.row: 2
                    Layout.column: 1
                    Layout.fillWidth: true
                    placeholderText: "可选"
                    text: contactData.email || ""
                    color: colors.text
                    inputMethodHints: Qt.ImhEmailCharactersOnly
                    validator: RegularExpressionValidator {
                        regularExpression: /^[^\s@]+@[^\s@]+\.[^\s@]+$/
                    }
                }

                // 地址
                Label {
                    text: "地址"
                    Layout.row: 3
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                TextField {
                    id: addressField
                    Layout.row: 3
                    Layout.column: 1
                    Layout.fillWidth: true
                    placeholderText: "可选"
                    text: contactData.address || ""
                    color: colors.text
                }

                // 备注
                Label {
                    text: "备注"
                    Layout.row: 4
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                TextArea {
                    id: notesField
                    Layout.row: 4
                    Layout.column: 1
                    Layout.fillWidth: true
                    Layout.preferredHeight: 80
                    placeholderText: "添加备注..."
                    text: contactData.notes || ""
                    color: colors.text
                    wrapMode: Text.Wrap
                }

                // 收藏状态
                Label {
                    text: "收藏"
                    Layout.row: 5
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                Switch {
                    id: favoriteSwitch
                    Layout.row: 5
                    Layout.column: 1
                    checked: contactData.favorite || false
                }

                // 分组
                Label {
                    text: "分组"
                    Layout.row: 6
                    Layout.column: 0
                    font.pointSize: 14
                    color: colors.text
                }

                ComboBox {
                    id: groupCombo
                    Layout.row: 6
                    Layout.column: 1
                    Layout.fillWidth: true
                    model: ["家人", "朋友", "同事", "同学", "其他"]
                    currentIndex: {
                        if (contactData.group) {
                            return model.indexOf(contactData.group)
                        }
                        return 4 // 默认"其他"
                    }
                }
            }

            // 表单验证状态
            Label {
                id: validationMessage
                Layout.fillWidth: true
                Layout.topMargin: 10
                visible: false
                color: "red"
                wrapMode: Text.Wrap
            }

            // 操作按钮
            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: 20
                spacing: 20

                Button {
                    text: "取消"
                    Layout.fillWidth: true
                    flat: true
                    onClicked: closeRequested()
                }

                Button {
                    id: saveButton
                    text: "保存"
                    Layout.fillWidth: true
                    highlighted: true
                    enabled: false
                    onClicked: saveContact()
                }
            }
        }
    }

    // 表单验证函数
    function validateForm() {
        var isValid = nameField.text.trim() !== "" && phoneField.text.trim() !== "";
        saveButton.enabled = isValid;

        if (!isValid) {
            if (nameField.text.trim() === "" && phoneField.text.trim() === "") {
                validationMessage.text = "姓名和电话是必填项";
            } else if (nameField.text.trim() === "") {
                validationMessage.text = "姓名是必填项";
            } else {
                validationMessage.text = "电话是必填项";
            }
            validationMessage.visible = true;
        } else {
            validationMessage.visible = false;
        }

        return isValid;
    }

    // 保存联系人
    function saveContact() {
        if (!validateForm()) return;

        if (contactIndex === -1) {
            // 添加新联系人
            contactModel.addContact(
                nameField.text.trim(),
                phoneField.text.trim(),
                emailField.text.trim(),
                addressField.text.trim(),
                notesField.text.trim(),
                favoriteSwitch.checked,
                groupCombo.currentText
            );
            saveCompleted(-1); // 新增联系人没有索引
        } else {
            // 更新联系人
            contactModel.updateContact(
                contactIndex,
                nameField.text.trim(),
                phoneField.text.trim(),
                emailField.text.trim(),
                addressField.text.trim(),
                notesField.text.trim(),
                favoriteSwitch.checked,
                groupCombo.currentText
            );
            saveCompleted(contactIndex);
        }

        closeRequested();
    }

    // 组件加载时自动聚焦第一个字段
    Component.onCompleted: {
        nameField.forceActiveFocus();
        validateForm();
    }
}
