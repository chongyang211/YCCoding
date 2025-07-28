import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: home

    ColumnLayout{

        anchors.fill: parent
        spacing: 0

        HomeHeaderView {
            id:layoutHeaderView
        }

        HomeContrainView{
            id:pageHomeView
        }

        HomeBottomView{
            id:layoutBottomView
        }
    }
}
