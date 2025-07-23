import QtQuick
import QtQuick.Controls

Item {


    // 通过注册的上下文属性访问
    Text {
        anchors.centerIn: parent
        text: "传感器数值: " + sensorData.value
        font.pixelSize: 24
    }

    // 可选的暂停按钮
    Button {
        anchors.bottom: parent.bottom
        text: "暂停"
        onClicked: sensorData.m_timer.stop()
    }
}
