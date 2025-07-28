import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {
    id: detailView

    // 连接到外部状态的属性
    required property int currentContactIndex
    property var contactData: currentContactIndex >= 0 ?
        contactModel.getContactDetails(currentContactIndex) :
        {"name": "", "phone": "", "email": "", "address": "", "notes": "", "favorite": false}

    // 状态属性
    property bool isFavorite: contactData.favorite || false
    property string contactName: contactData.name || ""
    property string contactPhone: contactData.phone || ""
    property string contactEmail: contactData.email || ""
    property string contactAddress: contactData.address || ""
    property string contactNotes: contactData.notes || ""
    property url contactAvatar: contactData.avatar || ""

    // 动态高度计算
    property real cardHeight: Math.max(detailsColumn.implicitHeight + 40, 350)

    // 主题颜色引用
    property QtObject colors: window.colors

    ScrollView {
        id: scrollView
        anchors.fill: parent
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        clip: true

        contentWidth: parent.width
        contentHeight: layoutColumn.implicitHeight

        ColumnLayout {
            id: layoutColumn
            width: scrollView.width - 20
            spacing: 25
            anchors.horizontalCenter: parent.horizontalCenter

            // 头像区域
            Item {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20

                Rectangle {
                    id: avatarBorder
                    width: 140
                    height: 140
                    radius: width / 2
                    border.color: colors.accent
                    border.width: 3
                    color: "transparent"

                    Image {
                        id: avatarImage
                        anchors.fill: parent
                        anchors.margins: 6
                        source: detailView.contactAvatar
                        fillMode: Image.PreserveAspectCrop
                        layer.enabled: true
                    }
                }

                // 收藏徽章
                Rectangle {
                    visible: isFavorite
                    anchors.top: avatarBorder.top
                    anchors.right: avatarBorder.right
                    width: 40
                    height: 40
                    radius: width / 2
                    color: "#FFD700"
                    border.color: colors.background
                    border.width: 2

                    Image {
                        anchors.centerIn: parent
                        source: "images/star.png"
                        width: 24
                        height: 24
                    }
                }
            }

            // 姓名和收藏按钮
            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.alignment: Qt.AlignHCenter

                Label {
                    text: detailView.contactName
                    font.bold: true
                    font.pointSize: 28
                    color: colors.text
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                }

                ToolButton {
                    icon.source: isFavorite ? "images/favorite_filled.png" : "images/favorite.png"
                    icon.color: isFavorite ? "gold" : colors.text
                    icon.width: 28
                    icon.height: 28
                    onClicked: {
                        if (detailView.currentContactIndex >= 0) {
                            contactModel.toggleFavorite(detailView.currentContactIndex)
                        }
                    }
                    ToolTip.text: isFavorite ? "取消收藏" : "标记收藏"
                    ToolTip.visible: hovered
                    Layout.alignment: Qt.AlignRight
                }
            }

            // 详细信息卡片
            Rectangle {
                id: detailsCard
                Layout.fillWidth: true
                height: cardHeight
                radius: 10
                color: colors.cardBackground

                ColumnLayout {
                    id: detailsColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 15

                    // 电话信息行
                    DetailField {
                        label: "电话"
                        value: detailView.contactPhone
                        iconSource: "images/phone.png"
                        copyable: true
                        actionIcon: "images/call.png"
                        onActionClicked: Qt.openUrlExternally("tel:" + value)
                    }

                    // 邮箱信息行
                    DetailField {
                        label: "邮箱"
                        value: detailView.contactEmail
                        iconSource: "images/email.png"
                        copyable: true
                        actionIcon: "images/send.png"
                        onActionClicked: Qt.openUrlExternally("mailto:" + value)
                    }

                    // 地址信息行
                    DetailField {
                        label: "地址"
                        value: detailView.contactAddress
                        iconSource: "images/location.png"
                        copyable: true
                        actionIcon: "images/map.png"
                        onActionClicked: Qt.openUrlExternally("geo:0,0?q=" + encodeURIComponent(value))
                    }

                    // 备注信息行
                    DetailField {
                        label: "备注"
                        value: detailView.contactNotes
                        iconSource: "images/notes.png"
                        copyable: true
                        wrapMode: Text.Wrap
                    }

                    // 附加信息（创建时间等）
                    Item {
                        Layout.fillWidth: true
                        Layout.topMargin: 15

                        Label {
                            text: "添加日期: " + Qt.formatDateTime(new Date(), "yyyy-MM-dd")
                            font.pointSize: 12
                            color: colors.text
                            opacity: 0.6
                        }
                    }
                }
            }

            // 操作按钮区域
            Flow {
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.bottomMargin: 20
                spacing: 15
                layoutDirection: Qt.LeftToRight
                flow: width > 600 ? Flow.LeftToRight : Flow.TopToBottom

                Button {
                    id: editButton
                    text: "编辑联系人"
                    icon.source: "images/edit.png"
                    onClicked: {
                        if (detailView.currentContactIndex >= 0) {
                            contactEditorLoader.contactIndex = detailView.currentContactIndex
                            contactEditorLoader.active = true
                        }
                    }
                    leftPadding: 20
                    rightPadding: 20
                    display: AbstractButton.TextBesideIcon
                }

                Button {
                    id: shareButton
                    text: "分享"
                    icon.source: "images/share.png"
                    onClicked: {
                        contactModel.showMessage("分享联系人: " + detailView.contactName)
                        // 实际应用中实现分享功能
                    }
                    display: AbstractButton.TextBesideIcon
                }

                Button {
                    id: deleteButton
                    text: "删除联系人"
                    icon.source: "images/delete.png"
                    flat: true
                    onClicked: deleteDialog.open()
                    display: AbstractButton.TextBesideIcon
                }
            }
        }
    }

    // 删除确认对话框
    // Dialog {
    //     id: deleteDialog
    //     title: "删除联系人"

    //     Label {
    //         width: parent.width
    //         wrapMode: Text.Wrap
    //         text: "确定要永久删除联系人 \"" + detailView.contactName + "\" 吗？此操作无法撤销。"
    //         color: colors.text
    //     }
    //     // 标准按钮
    //     standardButtons: Dialog.Ok | Dialog.Cancel

    //     onAccepted: {
    //         contactModel.removeContact(detailView.currentContactIndex)
    //         window.currentView = 0 // 返回主视图
    //     }
    // }

    // 当详情数据变化时平滑过渡
    Behavior on contactName {
        PropertyAnimation { duration: 150 }
    }

    Behavior on contactAvatar {
        PropertyAnimation { duration: 250 }
    }
}
