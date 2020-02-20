import QtQuick 2.14
import ChatObject 1.0

ChatItem {
    content: Rectangle {
        id: message
        radius: appTheme.bigRadius
        width: contentReadWidth + 1
        height: msgText.implicitHeight
        clip: true

        Rectangle {
            id: corner
            width: message.radius
            height: message.radius
            color: message.color
        }

        TextEdit {
            id: msgText
            text: chatItem.text
            readOnly: true
            selectByMouse: true
            padding: appTheme.stdSpacing
            wrapMode: Text.WordWrap
            font.pixelSize: appTheme.stdTextSize
            selectedTextColor: appTheme.backgroundColor
            selectionColor: appTheme.primaryActiveColor1
            width: contentReadWidth
        }
    }

    Component.onCompleted: {
        if (chatItem.sender.roleType !== ChatObject.Me) {
            message.color = appTheme.leftBubbleColor
            msgText.color = appTheme.leftBubbleTextColor
            corner.anchors.left = message.left
        } else {
            message.color = appTheme.rightBubbleColor
            msgText.color = appTheme.rightBubbleTextColor
            corner.anchors.right = message.right
        }

        corner.y = 0
    }
}
