import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: window
    width: 1200
    height: 800
    visible: true

    // 设置 Fusion 样式
    title: qsTr("Cloud Music Player")

    property int mWINDOW_WIDTH: 1200
    property int mWINDOW_HEIGHT: 800

    // StackView for page navigation
    StackView {
        id: stackView
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        initialItem: HomePage {
            id: home
        }
        // 禁用所有动画
        pushEnter: null
        pushExit: null
        popEnter: null
        popExit: null
        replaceEnter: null
        replaceExit: null

        // 监听当前页面变化，更新状态栏颜色
        onCurrentItemChanged: {
            console.log("main StackView page changed: " + (currentItem ? currentItem.objectName || "Unknown" : "null"));
            console.log("main Page type: " + (currentItem ? (currentItem.constructor ? currentItem.constructor.name : "Unknown") : "null"));
            console.log("main Status bar color updated to: " + currentItem.statusBarColor);
            console.log("main Status bar color reset to default:#FFFFFF");
        }

        Component.onCompleted: {
            console.log("main StackView onCompleted")
            // 在 StackView 初始化完成后，立即跳转到 HomePage
            stackView.push("HomePage.qml")
        }
    }

    // 页面监听
    Component.onCompleted: {
        console.log("main onCompleted")
        //stackView.push("HomePage.qml");
    }

}
