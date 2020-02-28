import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: 16
    height: 16

    property alias source: iconImg.source
    property alias color: shader.color

    Image {
        id: iconImg
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        sourceSize.width: width
        sourceSize.height: height
        visible: false

        onStatusChanged: {
            if (status === Image.Ready)
                shader.visible = true
        }
    }

    // 如果不支持OpenGL则不适用
    ColorOverlay {
        id: shader
        anchors.fill: iconImg
        source: iconImg
        visible: false
    }
}
