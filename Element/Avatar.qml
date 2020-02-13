import QtQuick 2.0
import QtGraphicalEffects 1.14
import ChatObject 1.0
import "qrc:/Element/"

Rectangle {
    property var onlineState: ChatObject.NoneState
    property alias imageSource: avatar.source

    width: 46
    height: 46
    radius: 23
    color: appTheme.subColor1

    onOnlineStateChanged: {
        dot.visible = true
        grayAvatar.visible = false

        switch (onlineState) {
        case ChatObject.Online:
            dot.color = appTheme.onlineStateColor
            break
        case ChatObject.Offline:
            dot.color = appTheme.offlineStateColor
            grayAvatar.visible = true
            break
        case ChatObject.Busy:
            dot.color = appTheme.busyStateColor
            break
        default:
            dot.visible = false
            break
        }
    }

    Image {
        id: avatar
        anchors.fill: parent
        visible: false
        sourceSize.width: parent.width
        sourceSize.height: parent.height

        Colorize {
            id: grayAvatar
            anchors.fill: avatar
            source: avatar
            hue: 0.0
            saturation: 0.0
            lightness: 0.0
            visible: false
        }
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        radius: width / 2
        visible: false
    }

    OpacityMask {
        anchors.fill: avatar
        source: avatar
        maskSource: mask
        visible: true
        antialiasing: true
    }

    Rectangle {
        id: dot
        width: 10
        height: 10
        radius: 5
        anchors {
            bottom: parent.bottom
            right: parent.right
        }
        visible: false
    }
}
