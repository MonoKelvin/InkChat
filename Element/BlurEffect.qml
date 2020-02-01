import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {
    property var background
    property var target

    property alias blurRadius: fastBlur.radius

    opacity: 0.2
    width: target.width
    height: target.height

    Item {
        id: blurItem
        clip: true
        anchors.fill: parent
        visible: false

        Image {
            id: toBlurImg
            width: background.width
            height: background.height
            x:0; y:0
        }
    }

    FastBlur {
        id: fastBlur
        source: blurItem
        width: blurItem.width
        height: blurItem.height
    }

    /**
     * 更新背景控件
     * @note 该方法应当在 当背景控件需要更新时 调用
     */
    function update() {
        if(background.width > 1 && background.height > 1) {
            background.grabToImage(function(result) {
                toBlurImg.source = result.url;
            }, Qt.size(background.width, background.height))
            // return toBlurImg
        }
        // return null
    }
}
