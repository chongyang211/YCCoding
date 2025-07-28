import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: fieldRoot
    property string label
    property string value
    property string iconSource
    property string actionIcon
    property bool copyable: false
    property int wrapMode: Text.NoWrap

    signal actionClicked

    Layout.fillWidth: true
    height: Math.max(60, valueLabel.implicitHeight + 30)

    RowLayout {
        anchors.fill: parent
        spacing: 15

        // 类型图标
        Image {
            source: iconSource
            sourceSize: Qt.size(28, 28)
            Layout.alignment: Qt.AlignTop
        }

        // 文本信息区
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            // 标签
            Label {
                text: label
                font.bold: true
                font.pointSize: 13
                color: colors.text
                opacity: 0.8
            }

            // 值
            Label {
                id: valueLabel
                text: value
                Layout.fillWidth: true
                font.pointSize: 15
                color: colors.text
                wrapMode: fieldRoot.wrapMode
                elide: wrapMode === Text.NoWrap ? Text.ElideRight : Text.ElideNone
            }
        }

        // 操作按钮
        ToolButton {
            visible: actionIcon !== ""
            icon.source: actionIcon
            icon.color: colors.accent
            icon.width: 28
            icon.height: 28
            onClicked: fieldRoot.actionClicked()
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            ToolTip.visible: hovered
            ToolTip.text: getActionTooltip()

            function getActionTooltip() {
                if (actionIcon.includes("call")) return "打电话"
                if (actionIcon.includes("send")) return "发送邮件"
                if (actionIcon.includes("map")) return "查看地图"
                if (actionIcon.includes("copy")) return "复制"
                return "操作"
            }
        }

        // 复制按钮
        ToolButton {
            visible: copyable && value !== ""
            icon.source: "images/copy.png"
            icon.color: colors.text
            icon.width: 24
            icon.height: 24
            onClicked: {
                copyToClipboard(value);
                contactModel.showMessage(label + "已复制")
            }
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            ToolTip.visible: hovered
            ToolTip.text: "复制"
        }
    }

    // 分隔线
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: colors.highlight
        opacity: 0.2
    }

    // 复制到剪贴板函数
    function copyToClipboard(text) {
        var textArea = Qt.createQmlObject('import QtQuick 2.0; TextArea {}', fieldRoot);
        textArea.text = text;
        textArea.selectAll();
        textArea.copy();
        textArea.destroy();
    }
}
