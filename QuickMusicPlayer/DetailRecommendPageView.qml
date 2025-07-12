import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Item {

    ScrollView{
        clip: true
        ColumnLayout {
            Text {
                text: qsTr("推荐内容")
                font.pointSize: 18
            }

            MusicBannerView{
                id:bannerView
                Layout.preferredWidth: window.width-200
                Layout.preferredHeight: (window.width-200)*0.3
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        Component.onCompleted: {
            getBannerList()
        }

        function getBannerList(){
            var reply = "{
    \"banner\": [{
            \"imageUrl\": \"https://img1.baidu.com/it/u=1804451539,124474196&fm=253&fmt=auto&app=138&f=JPEG?w=678&h=380\"
        },
        {
            \"imageUrl\": \"https://pic.quanjing.com/ci/ul/QJ9126352199.jpg?x-oss-process=style/350h\"
        },
        {
            \"imageUrl\": \"https://cdn.colorhub.me/QgpUMkZxNhU/rs:auto:0:500:0/g:ce/fn:colorhub/bG9jYWw6Ly8vZmIvNmYvMjlkMTE1NjRkNmI5ZmRhOTczYmU3ZmUyNmMyMDkwM2MwZjU5ZmI2Zi5qcGVn.webp\"
        }
    ]
}";
            var banners = JSON.parse(reply).banner
            bannerView.bannerList = banners
        }
    }

}
