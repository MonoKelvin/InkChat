import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {
    property var background
    property var show

    property alias width: blurItem.width
    property alias height: blurItem.height

    property alias blurRadius: fastBlur.radius

    width: sourceItem.width
    height: sourceItem.height
    blurRadius: 64

    Item {
        id: blurItem
        clip: true
        visible: false

        Image {
           id: toBlurImg
           x:0; y:0
        }
    }

    FastBlur {
        id: fastBlur
        source: blurItem
        width: blurItem.width
        height: blurItem.height
    }

    function update() {
        background.grabToImage(function(result) {
            toBlurImg.source = result.url;
        }, Qt.size(width, height))
    }
}
