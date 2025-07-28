import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Item {
    // 属性保存当前视图和主题
    property int currentView: 0 // 0:列表视图, 1:网格视图, 2:详情视图
    property int contactIndex: -1
    property string theme: "light" // "light" or "dark"
    property bool showFavorites: false

    // 为不同主题定义颜色
    QtObject {
        id: lightTheme
        property color background: "#f0f0f0"
        property color cardBackground: "#ffffff"
        property color text: "#333333"
        property color accent: "#4285f4"
        property color highlight: "#f5f5f5"
    }

    QtObject {
        id: darkTheme
        property color background: "#303030"
        property color cardBackground: "#424242"
        property color text: "#ffffff"
        property color accent: "#8ab4f8"
        property color highlight: "#616161"
    }

    // 当前主题引用
    property QtObject colors: theme === "light" ? lightTheme : darkTheme

    // 设置应用程序背景
    Rectangle {
        anchors.fill: parent
        color: colors.background
    }

    // 顶部工具栏
    ToolBar {
        id: toolbar
        width: parent.width
        height: 60

        background: Rectangle {
            color: colors.accent
        }

        RowLayout {
            anchors.fill: parent
            spacing: 20

            // 返回按钮（仅详情视图显示）
            ToolButton {
                visible: window.currentView === 2
                text: "返回"
                font.pointSize: 12
                onClicked: window.currentView = 0
            }

            // 应用标题
            Label {
                text: "联系人管理器"
                font.bold: true
                font.pointSize: 16
                color: "white"
                Layout.fillWidth: true
            }

            // 视图切换按钮
            ToolButton {
                id: viewToggleButton
                text: window.currentView === 1 ? "列表视图" : "网格视图"
                onClicked: window.currentView = window.currentView === 1 ? 0 : 1
                visible: window.currentView !== 2
            }

            // 收藏过滤按钮
            ToolButton {
                icon.source: "images/favorite.png"
                icon.color: showFavorites ? "yellow" : "white"
                onClicked: showFavorites = !showFavorites
                ToolTip.text: showFavorites ? "显示所有联系人" : "仅显示收藏"
                ToolTip.visible: hovered
                visible: window.currentView !== 2
            }

            // 添加联系人按钮
            ToolButton {
                icon.source: "images/add.png"
                onClicked: {
                    contactIndex = -1; // 表示新联系人
                    contactEditorLoader.active = true
                }
                ToolTip.text: "添加联系人"
                ToolTip.visible: hovered
                visible: window.currentView !== 2
            }

            // 主题切换按钮
            ToolButton {
                icon.source: theme === "light" ? "images/moon.png" : "images/sun.png"
                onClicked: theme = theme === "light" ? "dark" : "light"
                ToolTip.text: theme === "light" ? "切换到深色主题" : "切换到浅色主题"
                ToolTip.visible: hovered
            }
        }
    }

    // 主要内容区域
    StackLayout {
        anchors.top: toolbar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: window.currentView

        // 列表视图
        Item {
            ListView {
                id: listView
                anchors.fill: parent
                clip: true
                model: showFavorites ? contactModel.filter(row => row.favorite) : contactModel

                delegate: Rectangle {
                    id: contactItem
                    width: listView.width
                    height: 80
                    color: colors.cardBackground
                    border.color: colors.highlight
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 15

                        // 头像
                        Rectangle {
                            width: 60
                            height: 60
                            radius: 30
                            color: "transparent"

                            Image {
                                anchors.fill: parent
                                source: model.avatar
                                fillMode: Image.PreserveAspectCrop
                                layer.enabled: true

                            }
                        }

                        // 联系人信息
                        ColumnLayout {
                            spacing: 4
                            Layout.fillWidth: true

                            Label {
                                text: model.name
                                font.bold: true
                                font.pointSize: 14
                                color: colors.text
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }

                            Label {
                                text: model.phone
                                font.pointSize: 12
                                color: colors.text
                                opacity: 0.8
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }

                        // 收藏图标
                        ToolButton {
                            icon.source: model.favorite ? "images/favorite_filled.png" : "images/favorite.png"
                            icon.color: model.favorite ? "gold" : colors.text
                            onClicked: contactModel.toggleFavorite(index)
                            ToolTip.text: model.favorite ? "取消收藏" : "标记收藏"
                            ToolTip.visible: hovered
                        }
                    }

                    // 点击事件
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            contactIndex = index;
                            window.currentView = 2;
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {}
            }

            // 没有联系人时的提示
            Label {
                anchors.centerIn: parent
                visible: listView.count === 0
                text: showFavorites ? "没有收藏的联系人" : "没有联系人"
                font.pointSize: 16
                color: colors.text
                opacity: 0.5
            }
        }

        // 网格视图
        GridView {
            id: gridView
            cellWidth: 180
            cellHeight: 200
            clip: true
            model: showFavorites ? contactModel.filter(row => row.favorite) : contactModel

            delegate: Rectangle {
                id: gridItem
                width: gridView.cellWidth - 10
                height: gridView.cellHeight - 10
                color: colors.cardBackground
                radius: 10
                border.color: colors.highlight
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    // 头像
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 80
                        height: 80
                        radius: 40
                        color: "transparent"

                        Image {
                            anchors.fill: parent
                            source: model.avatar
                            fillMode: Image.PreserveAspectCrop
                            layer.enabled: true

                        }
                    }

                    // 联系人姓名
                    Label {
                        text: model.name
                        font.bold: true
                        font.pointSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        color: colors.text
                        elide: Text.ElideRight
                    }

                    // 电话
                    Label {
                        text: model.phone
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        color: colors.text
                        opacity: 0.8
                        elide: Text.ElideRight
                    }

                    // 收藏图标
                    ToolButton {
                        Layout.alignment: Qt.AlignRight
                        icon.source: model.favorite ? "images/favorite_filled.png" : "images/favorite.png"
                        icon.color: model.favorite ? "gold" : colors.text
                        onClicked: contactModel.toggleFavorite(index)
                        ToolTip.text: model.favorite ? "取消收藏" : "标记收藏"
                        ToolTip.visible: hovered
                    }
                }

                // 点击事件
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contactIndex = index;
                        window.currentView = 2;
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        // 联系人详情视图
        ContactDetailView {
            id: detailView
            currentContactIndex: window.contactIndex
        }
    }

    // 状态栏
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 30
        color: colors.highlight

        Label {
            anchors.centerIn: parent
            text: "联系人总数: " + contactModel.count + (showFavorites ? " (仅显示收藏)" : "")
            color: colors.text
        }
    }

    // 联系人编辑弹窗加载器
    Loader {
        id: contactEditorLoader
        active: false
        anchors.centerIn: parent
        sourceComponent: ContactEditor {}
    }

    // 联系人详情视图组件
    Component {
        id: detailViewComponent

        ScrollView {
            id: detailScroll
            anchors.fill: parent
            contentWidth: width

            ColumnLayout {
                width: detailScroll.width
                spacing: 20
                anchors.margins: 30

                // 头像
                Item {
                    Layout.alignment: Qt.AlignHCenter
                    width: 120
                    height: 120

                    Rectangle {
                        anchors.fill: parent
                        radius: width / 2
                        color: "transparent"
                        border.width: 2
                        border.color: colors.accent

                        Image {
                            anchors.fill: parent
                            anchors.margins: 4
                            source: detailView.contactAvatar
                            fillMode: Image.PreserveAspectCrop
                            layer.enabled: true
                        }
                    }
                }

                // 姓名和收藏
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 20

                    Label {
                        text: detailView.contactName
                        font.bold: true
                        font.pointSize: 24
                        color: colors.text
                    }

                    ToolButton {
                        icon.source: detailView.contactFavorite ? "images/favorite_filled.png" : "images/favorite.png"
                        icon.color: detailView.contactFavorite ? "gold" : colors.text
                        onClicked: contactModel.toggleFavorite(contactIndex)
                        ToolTip.text: detailView.contactFavorite ? "取消收藏" : "标记收藏"
                        ToolTip.visible: hovered
                    }
                }

                // 详细信息卡片
                Rectangle {
                    Layout.fillWidth: true
                    Layout.leftMargin: 50
                    Layout.rightMargin: 50
                    height: 300
                    color: colors.cardBackground
                    radius: 10

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 15

                        // 电话
                        DetailField {
                            label: "电话"
                            value: detailView.contactPhone
                            iconSource: "images/phone.png"
                            onActionClicked: {
                                Qt.openUrlExternally("tel:" + value)
                            }
                        }

                        // 邮箱
                        DetailField {
                            label: "邮箱"
                            value: detailView.contactEmail
                            iconSource: "images/email.png"
                            onActionClicked: Qt.openUrlExternally("mailto:" + value)
                        }

                        // 地址
                        DetailField {
                            label: "地址"
                            value: detailView.contactAddress
                            iconSource: "images/location.png"
                            onActionClicked: Qt.openUrlExternally("geo:0,0?q=" + encodeURIComponent(value))
                        }

                        // 备注
                        DetailField {
                            label: "备注"
                            value: detailView.contactNotes
                            iconSource: "images/notes.png"
                        }
                    }
                }

                // 操作按钮
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 20

                    Button {
                        text: "编辑"
                        icon.source: "images/edit.png"
                        onClicked: {
                            contactEditorLoader.active = true;
                        }
                    }

                    Button {
                        text: "删除"
                        icon.source: "images/delete.png"
                        onClicked: {
                            deleteDialog.open();
                        }
                    }

                    Button {
                        text: "分享"
                        icon.source: "images/share.png"
                        onClicked: contactModel.showMessage("分享联系人: " + detailView.contactName)
                    }
                }
            }
        }
    }

    // 详情视图组件
    Component {
        id: contactEditorComponent

        Rectangle {
            id: editorDialog
            width: 500
            height: 600
            radius: 10
            color: colors.cardBackground

            // 关闭按钮
            ToolButton {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 10
                icon.source: "images/close.png"
                onClicked: contactEditorLoader.active = false
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
                        text: contactIndex == -1 ? "添加新联系人" : "编辑联系人"
                        font.bold: true
                        font.pointSize: 18
                        Layout.alignment: Qt.AlignHCenter
                        color: colors.text
                    }

                    // 头像选择（简化版）
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 100
                        height: 100
                        radius: 50
                        color: "transparent"
                        border.width: 2
                        border.color: colors.accent

                        Image {
                            anchors.fill: parent
                            anchors.margins: 4
                            source: avatarField.image
                            fillMode: Image.PreserveAspectCrop
                            layer.enabled: true
                        }

                        ToolButton {
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            icon.source: "images/edit.png"
                            onClicked: contactModel.showMessage("选择自定义头像")
                        }
                    }

                    // 姓名
                    TextField {
                        id: nameField
                        Layout.fillWidth: true
                        placeholderText: "姓名"
                        text: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).name : ""
                        color: colors.text
                    }

                    // 电话
                    TextField {
                        id: phoneField
                        Layout.fillWidth: true
                        placeholderText: "电话"
                        text: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).phone : ""
                        color: colors.text
                        inputMethodHints: Qt.ImhDialableCharactersOnly
                    }

                    // 邮箱
                    TextField {
                        id: emailField
                        Layout.fillWidth: true
                        placeholderText: "邮箱"
                        text: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).email : ""
                        color: colors.text
                        inputMethodHints: Qt.ImhEmailCharactersOnly
                    }

                    // 地址
                    TextField {
                        id: addressField
                        Layout.fillWidth: true
                        placeholderText: "地址"
                        text: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).address : ""
                        color: colors.text
                    }

                    // 备注
                    TextField {
                        id: notesField
                        Layout.fillWidth: true
                        placeholderText: "备注"
                        text: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).notes : ""
                        color: colors.text
                    }

                    // 收藏状态
                    Switch {
                        id: favoriteSwitch
                        text: "收藏"
                        checked: contactIndex != -1 ? contactModel.getContactDetails(contactIndex).favorite : false
                        Layout.alignment: Qt.AlignLeft
                    }

                    // 操作按钮
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 20

                        Button {
                            text: "取消"
                            Layout.fillWidth: true
                            onClicked: contactEditorLoader.active = false
                        }

                        Button {
                            text: "保存"
                            Layout.fillWidth: true
                            highlighted: true
                            onClicked: {
                                if (contactIndex == -1) {
                                    // 添加新联系人
                                    contactModel.addContact(nameField.text,
                                                            phoneField.text,
                                                            emailField.text,
                                                            addressField.text,
                                                            notesField.text,
                                                            favoriteSwitch.checked);
                                } else {
                                    // 更新联系人
                                    contactModel.updateContact(contactIndex,
                                                               nameField.text,
                                                               phoneField.text,
                                                               emailField.text,
                                                               addressField.text,
                                                               notesField.text,
                                                               favoriteSwitch.checked);
                                }
                                contactEditorLoader.active = false;
                            }
                        }
                    }
                }
            }
        }
    }
}
