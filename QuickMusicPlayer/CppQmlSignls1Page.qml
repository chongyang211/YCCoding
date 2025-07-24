import QtQuick
import QtQuick.Controls

Item {
    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: temperatureDisplay
            text: "当前温度: -- °C"
            font.pixelSize: 24
        }

        Button {
            text: "更新温度"
            onClicked: sensorMonitor.update() // 触发更新
        }

        // 信号处理
        Connections {
            target: sensorMonitor
            function onTemperatureChanged(value) {
                temperatureDisplay.text = "当前温度: " + value.toFixed(1) + " °C"
            }
        }

        Component.onCompleted: {
            // 检查对象是否存在
            console.log("sensorMonitor 对象存在:", sensorMonitor !== undefined)
            console.log("对象类型:", typeof sensorMonitor)
            // 检查 update 属性类型
            console.log("update 类型:", typeof sensorMonitor.update)
            // 检查对象原型
            console.log("对象属性:", Object.keys(sensorMonitor))
        }
    }
}
