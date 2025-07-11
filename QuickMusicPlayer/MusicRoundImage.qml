import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
// import QtGraphicalEffects

Item {

    property string imgSrc: "qrc:/images/player"
    property int borderRadius: 5

    Image{
        id:image
        anchors.centerIn: parent
        source:imgSrc
        smooth: true
        visible: false
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectCrop
        antialiasing: true
    }

    Rectangle{
        id:mask
        color: "black"
        anchors.fill: parent
        radius: borderRadius
        visible: false
        smooth: true
        antialiasing: true
    }

    // OpacityMask{
    //     anchors.fill:image
    //     //源图像，可以是 Image、Rectangle 或其他可视组件。
    //     source: image
    //     //遮罩图像，通常是带有透明度的图像（如 PNG）。
    //     maskSource: mask
    //     visible: true
    //     antialiasing: true
    // }

    // 使用 Canvas 实现遮罩
    // Canvas {
    //     anchors.fill: parent
    //     onPaint: {
    //         var ctx = getContext("2d");

    //         // 绘制源图像
    //         ctx.drawImage(sourceImage, 0, 0, width, height);

    //         // 设置遮罩
    //         ctx.globalCompositeOperation = "destination-in";
    //         ctx.drawImage(maskImage, 0, 0, width, height);
    //     }

    //     Component.onCompleted: {
    //         requestPaint();
    //     }
    // }
}
