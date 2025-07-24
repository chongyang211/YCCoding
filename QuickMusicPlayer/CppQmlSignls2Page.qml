import QtQuick
import QtQuick.Controls

Item {
    Column {
        anchors.centerIn: parent
        spacing: 20

        TextField {
            id: usernameField
            placeholderText: "用户名"
            width: 200
        }

        TextField {
            id: passwordField
            placeholderText: "密码"
            echoMode: TextInput.Password
            width: 200
        }

        Button {
            text: "登录"
            width: 200
            onClicked: {
                // 调用 C++ 槽函数
                controller.handleLogin(usernameField.text, passwordField.text)
            }
        }

        // 处理登录结果
        Connections {
            target: controller
            function onLoginResult(success, message) {
                if (success) {
                    loginStatus.text = message
                    loginStatus.color = "green"
                    // 导航到主界面...
                } else {
                    loginStatus.text = message
                    loginStatus.color = "red"
                }
            }
        }

        Text {
            id: loginStatus
            color: "black"
        }
    }
}
