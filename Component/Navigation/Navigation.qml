import QtQuick 2.1
import QtQuick.Controls 2.4
import "qrc:/Element/"

Rectangle {
    property int currentIndex: -1
    property alias userAvatar: avatar.imageSource
    property alias nickName: userName.text

    width: 70
    color: appTheme.backgroundColor

    signal navigate(var index)

    Component.onCompleted: {
        btnMessage.checked = true
    }

    onCurrentIndexChanged: {
        navigate(currentIndex)
        navBgBlock.y = navigation.y + currentIndex * (40 + navigation.spacing)
    }

    Rectangle {
        id: navBgBlock
        width: 40
        height: 40
        color: appTheme.subColor1
        radius: appTheme.stdRadius
        anchors.horizontalCenter: parent.horizontalCenter

        Behavior on y {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutQuint
            }
        }
    }

    Column {
        id: navigation
        spacing: appTheme.narrowSpacing
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        StyleButton {
            id: btnMessage
            width: 40
            height: 40
            hoverEnabled: false
            pressedColor: "transparent"
            checkable: true
            autoExclusive: true
            icon.name: "message"
            icon.color: checked ? appTheme.primaryColor1 : "transparent"
            onCheckedChanged: if (checked) currentIndex = 0
        }
        StyleButton {
            id: btnFriend
            width: 40
            height: 40
            hoverEnabled: false
            pressedColor: "transparent"
            checkable: true
            autoExclusive: true
            icon.name: "people"
            icon.color: checked ? appTheme.primaryColor1 : "transparent"
            onCheckedChanged: if (checked) currentIndex = 1
        }
        StyleButton {
            id: btnSettings
            width: 40
            height: 40
            hoverEnabled: false
            pressedColor: "transparent"
            checkable: true
            autoExclusive: true
            icon.name: "settings"
            icon.color: checked ? appTheme.primaryColor1 : "transparent"
            onCheckedChanged: if (checked) currentIndex = 2
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
