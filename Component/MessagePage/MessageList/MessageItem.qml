﻿import QtQuick 2.0
import User 1.0
import "qrc:/Element"

Item {
    property int msgId: -1
    property int unreadMsgNumber: -1
    property bool hasRead: true
    property string msgTime: new Date().toString()

    height: 70

    Component.onCompleted: {
        msgId = id;
        hasRead = read;
        nameText.text = name;
        messageText.text = message;
        msgTime = messageTime;
        unreadMsgNumber = unreadMessageNumber;
    }

    onHasReadChanged: {
        if(hasRead) {
            unreadMsgRect.width = 0;
            unreadMsgRect.visible = false;
        } else {
            if (unreadMsgNumber <= 0) {
                return;
            }

            unreadMsgRect.visible = true;
            unreadMsgRect.width = unreadMsgNumText.contentWidth + 14
        }
    }

    onUnreadMsgNumberChanged: {
        if(hasRead) return;

        console.log(unreadMsgNumber)
        if(unreadMsgNumber <= 0) {
            unreadMsgRect.width = 0;
            unreadMsgRect.visible = false;
            return;
        }

        unreadMsgRect.visible = true;
        if(unreadMsgNumber <= 99) {
            unreadMsgNumText.text = unreadMsgNumber;
        } else {
            unreadMsgNumText.text = "99+";
        }
        unreadMsgRect.width = unreadMsgNumText.contentWidth + 14
    }

    Avatar {
        id: avatar
        imageSource: avatarUrl
        onlineState: loginState
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: appTheme.stdSpacing
        onClicked: console.log("avatar clicked")

        onOnlineStateChanged: {
            switch(onlineState) {
            case User.Online:
                nameText.color = appTheme.mainTextColor;
                unreadMsgRect.color = appTheme.subColor3;
                unreadMsgNumText.color = appTheme.primaryColor3;
                break;
            case User.Offline:
                nameText.color = appTheme.subTextColor;
                unreadMsgRect.color = appTheme.tintColor;
                unreadMsgNumText.color = appTheme.subTextColor;
                break;
            }
        }
    }

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: avatar.right
        anchors.leftMargin: appTheme.stdSpacing
        anchors.right: unreadMsgRect.left
        anchors.rightMargin: appTheme.stdSpacing
        spacing: appTheme.tinySpacing

        Text {
            id: nameText
            font.bold: true
            font.pixelSize: appTheme.stdTextSize
            color: appTheme.mainTextColor
            width: parent.width
            elide: Text.ElideRight
        }
        Text {
            id: messageText
            font.pixelSize: appTheme.smallTextSize
            color: appTheme.subTextColor
            width: parent.width
            elide: Text.ElideRight
        }
    }

    Rectangle {
        id: unreadMsgRect
        color: appTheme.subColor3
        height: appTheme.stdBadgeHeight
        radius: appTheme.stdBadgeHeight / 2
        width: unreadMsgNumText.contentWidth + 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: appTheme.stdSpacing

        Text {
            id: unreadMsgNumText
            font.pixelSize: appTheme.smallTextSize
            anchors.centerIn: parent
            color: appTheme.primaryColor3
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: console.log("message item clicked")
    }
}
