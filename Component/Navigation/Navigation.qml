import QtQuick 2.1
import QtQuick.Controls 2.4
import "qrc:/Element/"

Rectangle {
    property int currentIndex: 0
    property alias userAvatar: avatar.imageSource
    property alias nickName: userName.text

    width: 70
    color: appTheme.backgroundColor
    onCurrentIndexChanged: navigate(currentIndex)

    signal navigate(var index)

    Column {
        id: navigation
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: {
            btnMessage.checked = true
        }

        StyleButton {
            id: btnMessage
            width: 40
            height: 40
            radius: appTheme.stdRadius
            checkable: true
            autoExclusive: true
            onCheckedChanged: if (checked)
                                  currentIndex = 0
        }
        StyleButton {
            id: btnFriend
            width: 40
            height: 40
            radius: appTheme.stdRadius
            checkable: true
            autoExclusive: true
            onCheckedChanged: if (checked)
                                  currentIndex = 1
        }
        StyleButton {
            id: btnMine
            width: 40
            height: 40
            radius: appTheme.stdRadius
            checkable: true
            autoExclusive: true
            onCheckedChanged: if (checked)
                                  currentIndex = 2
        }
        StyleButton {
            id: btnSettings
            width: 40
            height: 40
            radius: appTheme.stdRadius
            checkable: true
            autoExclusive: true
            onCheckedChanged: if (checked)
                                  currentIndex = 3
        }
    }

    Avatar {
        id: avatar
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: userName.top
            bottomMargin: appTheme.narrowSpacing
        }
    }

    Text {
        id: userName
        font.pixelSize: appTheme.smallTextSize
        color: appTheme.mainTextColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.NoWrap
        elide: Text.ElideRight
        width: avatar.width
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: appTheme.largeSpacing
        }
    }
}
