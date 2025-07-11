import QtQuick

Item {
    Text {
        text: "我喜欢的"
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

}
