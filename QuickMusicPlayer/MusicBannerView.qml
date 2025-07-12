import QtQuick
import QtQuick.Controls

Frame{
    property int current: 0
    property var bannerList: []

    MusicRoundImage{
        id:leftImage
        width:parent.width*0.6
        height: parent.height*0.8
        anchors{
            left: parent.left
            bottom: parent.bottom
            bottomMargin: 20
        }

        imgSrc: getLeftImgSrc()
    }

    MusicRoundImage{
        id:centerImage
        width:parent.width*0.6
        height: parent.height
        z:2
        anchors.centerIn: parent
        imgSrc: getCenterImgSrc()
    }

    MusicRoundImage{
        id:rightImage
        width:parent.width*0.6
        height: parent.height*0.8
        anchors{
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 20
        }
        imgSrc: getRightImgSrc()
    }

    function getLeftImgSrc(){
        var url = bannerList.length?bannerList[(current-1+bannerList.length)%bannerList.length].imageUrl:"";
        console.log("getLeftImgSrc: " + url)
        return url
    }
    function getCenterImgSrc(){
        var url = bannerList.length?bannerList[current].imageUrl:""
        console.log("getCenterImgSrc: " + url)
        return url
    }
    function getRightImgSrc(){
        var url = bannerList.length?bannerList[(current+1+bannerList.length)%bannerList.length].imageUrl:""
        console.log("getRightImgSrc: " + url)
        return url
    }
}
