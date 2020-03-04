import QtQuick 2.14
import ChatObject 1.0
import QtGraphicalEffects 1.14

ChatItem {
    id: root

    property alias imageSource: image.source

    imageSource: "file:///E:/图片/壁纸/WallPaper (1).jpg"

    content: Item {
        id: bubbleRect
        width: 128
        height: 128

        Image {
            id: image
            anchors.fill: parent
            sourceSize.width: parent.width
            sourceSize.height: parent.height
            visible: false
            fillMode: Image.PreserveAspectFit

            onStatusChanged: {
                switch (image.status) {
                case Image.Ready:
                    loadingText.visible = false
                    opacityMask.visible = true
                    break
                case Image.Error:
                    loadingText.text = qsTr("加载失败！")
                    break
                }
            }
        }

        Rectangle {
            id: mask
            anchors.fill: parent
            color: appTheme.tintColor
            radius: appTheme.bigRadius

            Text {
                id: loadingText
                text: qsTr("加载中") + "..."
                anchors.centerIn: parent
                font.pixelSize: appTheme.smallTextSize
                color: appTheme.subTextColor
            }
        }

        OpacityMask {
            id: opacityMask
            anchors.fill: image
            source: image
            maskSource: mask
            antialiasing: true
            visible: false
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (image.status === Image.Ready) {
                    var component = Qt.createComponent("qrc:/ImageBrowser.qml")

                    if (component.status === Component.Ready) {
                        var page = component.createObject(window)
                        page.anchors.fill = window.contentItem
                        page.imageSource = root.imageSource
                    }
                    // todo: 加载图片查看器
                }
            }
        }
    }
}
