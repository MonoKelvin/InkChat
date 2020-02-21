import QtQuick 2.14
import ChatObject 1.0

ChatItem {
    content: Rectangle {
        id: message
        radius: appTheme.bigRadius
        width: msgText.width
        height: msgText.implicitHeight
        clip: true

        Rectangle {
            id: corner
            y: 0
            width: message.radius
            height: message.radius
            color: message.color
        }

        TextEdit {
            id: msgText
            text: chatItem.text
            readOnly: true
            selectByMouse: true
            font.pixelSize: appTheme.stdTextSize
            selectedTextColor: appTheme.backgroundColor
            selectionColor: appTheme.primaryActiveColor1
            wrapMode: Text.Wrap
            padding: appTheme.stdSpacing
            width: Math.min(contentMaxWidth, implicitWidth)
        }
    }

    Component.onCompleted: {
        if (chatItem.sender.roleType === ChatObject.Me) {
            message.color = appTheme.rightBubbleColor
            msgText.color = appTheme.rightBubbleTextColor
            corner.anchors.right = message.right
        } else {
            message.color = appTheme.leftBubbleColor
            msgText.color = appTheme.leftBubbleTextColor
            corner.anchors.left = message.left
        }
    }
}
