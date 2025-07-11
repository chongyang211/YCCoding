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
    title: qsTr("Cloud Music Player")

    property int mWINDOW_WIDTH: 1200
    property int mWINDOW_HEIGHT: 800

    ColumnLayout{

        anchors.fill: parent
        spacing: 0

        LayoutHeaderView {
            id:layoutHeaderView
        }

        PageHomeView{
            id:pageHomeView
        }

        LayoutBottomView{
            id:layoutBottomView
        }

    }


    // 页面监听
    Component.onCompleted: {
        console.log("Resource path:", "qrc:/images/music.png")

    }

}
