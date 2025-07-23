import QtQuick
import QtQuick.Controls

Item {
    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: usernameField
            placeholderText: "Username"
        }

        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        Button {
            text: "Login"
            onClicked: {
                if (authManager.login(usernameField.text, passwordField.text)) {
                    console.log("Login successful!");
                } else {
                    console.log("Login failed!");
                }
            }
        }
    }
}
