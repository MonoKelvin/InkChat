import QtQuick 2.0
import User 1.0
import "qrc:/Element/"

Item {
    property int size: 45

    property var onlineState: User.NoneState

    property alias imageSource: image.source
    property url defaultToShow: "qrc:/AppResource/Icon/default_avatar.png"

    clip: true
    width: size
    height: size

    signal clicked

    Component.onCompleted: {
        if(image.status !== Image.Ready) {
            image.source = defaultToShow
        }
    }

    function getDefaultUrl() {
        return "qrc:/AppResource/Icon/default_avatar.png";
    }

    onOnlineStateChanged: {
        dot.visible = true;
        switch(onlineState) {
        case User.Online:
            dot.color = appTheme.onlineStateColor;
            break;
        case User.Offline:
            dot.color = appTheme.offlineStateColor;
            break;
        case User.Busy:
            dot.color = appTheme.busyStateColor;
            break;
        default:
            dot.visible = false;
            break;
        }
    }

    Rectangle {
        id: avatar
        anchors.fill: parent
        radius: size / 2

        Image {
            id: image
            anchors.fill: parent
        }

        Rectangle {
            id: dot
            width: 10;
            height: 10;
            radius: 5;
            anchors.bottom: avatar.bottom;
            anchors.right:avatar.right;
            visible: false
        }

        MouseArea {
            onPressed: clicked()
        }
    }
}
