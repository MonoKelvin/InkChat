import QtQuick 2.0
import User 1.0
import "qrc:/Element"

Rectangle {
    property int msgId: -1
    property int unreadMsgNumber: -1
    property bool hasRead: true
    property string message
    property string msgTime: new Date().toString()

    height: 70
    color: (messageListView.currentIndex === index) ? appTheme.widgetColor : "transparent"

    Component.onCompleted: {
        msgId = _id
        hasRead = _read
        msgTime = _messageTime
        unreadMsgNumber = _unreadMessageNumber
        message = _message
    }

    onHasReadChanged: {
        if (hasRead) {
            unreadMsgRect.width = 0
            unreadMsgRect.visible = false
        } else {
            if (unreadMsgNumber <= 0) {
                return
            }

            unreadMsgRect.visible = true
            unreadMsgRect.width = unreadMsgRect.contentWidth + 10
        }
    }

    onUnreadMsgNumberChanged: {
        if (hasRead)
            return

        if (unreadMsgNumber <= 0) {
            unreadMsgRect.width = 0
            unreadMsgRect.visible = false
            return
        }

        unreadMsgRect.visible = true
        if (unreadMsgNumber <= 99) {
            unreadMsgRect.text = unreadMsgNumber
        } else {
            unreadMsgRect.text = "99+"
        }
        unreadMsgRect.width = unreadMsgRect.contentWidth + 14
    }

    Avatar {
        id: avatar
        imageSource: _avatar
        onlineState: _loginState
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: appTheme.stdSpacing

        onOnlineStateChanged: {
            switch (onlineState) {
            case User.Online:
                nameText.color = appTheme.mainTextColor
                unreadMsgRect.color = appTheme.subColor3
                unreadMsgRect.textColor = appTheme.primaryColor3
                break
            case User.Offline:
                nameText.color = appTheme.subTextColor
                unreadMsgRect.color = appTheme.tintColor
                unreadMsgRect.textColor = appTheme.subTextColor
                break
            }
        }
    }

    Column {
        id: column_name_msg
        height: parent.height - appTheme.wideSpacing
        spacing: appTheme.tinySpacing
        anchors {
            left: avatar.right
            leftMargin: appTheme.stdSpacing
            verticalCenter: parent.verticalCenter
            right: column_time_badge.left
            rightMargin: appTheme.tinySpacing
        }

        Text {
            id: nameText
            width: parent.width
            font.bold: true
            verticalAlignment: Text.AlignBottom
            font.pixelSize: appTheme.stdTextSize
            color: appTheme.mainTextColor
            elide: Text.ElideRight
            text: _name
        }
        Text {
            id: messageText
            width: parent.width
            font.pixelSize: appTheme.smallTextSize
            color: appTheme.subTextColor
            elide: Text.ElideRight
            text: message
        }
    }

    Item {
        id: column_time_badge
        height: column_name_msg.height
        width: Math.max(timeText.implicitWidth, unreadMsgRect.implicitWidth)
        anchors {
            right: parent.right
            rightMargin: appTheme.stdSpacing
            verticalCenter: parent.verticalCenter
        }

        Text {
            id: timeText
            verticalAlignment: Text.AlignBottom
            font.pixelSize: appTheme.smallTextSize
            color: appTheme.subTextColor
            text: msgTime
            y: nameText.y
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Badge {
            id: unreadMsgRect
            text: unreadMsgNumber
            y: messageText.y
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            messageListView.currentIndex = index
            messageList.itemClicked(msgId)
        }
    }
}
