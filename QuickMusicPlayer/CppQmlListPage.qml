import QtQuick

Item {

    ListView {
        width: 200; height: 250
        model: dataModel  // 从 C++ 注册的模型实例

        delegate: Item {
            width: ListView.view.width
            height: 40

            // 通过 roleNames() 映射的名称访问
            Text {
                text: model.name  // 对应 C++ 中的 NameRole
                anchors.centerIn: parent
            }
        }
    }

}
