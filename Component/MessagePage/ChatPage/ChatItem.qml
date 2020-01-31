import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import ChatView 1.0
import "qrc:/Element/"

Item {
    id: bubbleItem

    /*
     * _avatar
     * _name
     * _message
     * _sendTime
     * _sendState : ChatView.ESendState
     * _sender : ChatView.ESender
     * _messageType
     */
    property string sendTime: {const t = new Date(); return t.getHours() + ":" + t.getMinutes()}
    property var sendState: ChatView.Sending

    onSendStateChanged: {
        // todo
        //        switch(sendState) {

        //        }
    }

    Avatar {
        id: avatar
        imageSource: _avatar
    }

    Text {
        id: name
        text: _name
        width: message.width
        elide: Text.ElideRight
        font.pixelSize: appTheme.stdTextSize
    }

    Rectangle {
        id: message
        radius: appTheme.bigRadius
        width: msgText.width + 2 * appTheme.narrowSpacing
        height: msgText.height + 2 * appTheme.narrowSpacing
        clip: true

        Rectangle {
            id: corner
            width: message.radius
            height: message.radius
            color: message.color
        }

        Text {
            id: msgText
            text: _message
            wrapMode: Text.WrapAnywhere
            font.pixelSize: appTheme.stdTextSize
            width: Math.min(bubbleItem.width * 0.7, msgText.contentWidth)
            anchors {
                left: parent.left
                top: parent.top
                leftMargin: appTheme.narrowSpacing
                topMargin: appTheme.narrowSpacing
            }
        }
    }

    Text {
        id: timeText
        text: _sendTime
        color: appTheme.subTextColor
        font.pixelSize: appTheme.smallTextSize
    }

    AnimatedImage {
        id: sendStateIcon
//        source: "D:/GraduationProject/InkChatQml/Resource/Icon/loading_15x.gif"
    }

    Component.onCompleted: {
        avatar.anchors.top = bubbleItem.top
        message.anchors.top = avatar.verticalCenter
        corner.anchors.top = message.top

        if(_sender !== ChatView.Me) {
            // 头像
            avatar.anchors.left = bubbleItem.left
            avatar.anchors.leftMargin = appTheme.stdSpacing

            // 消息
            message.anchors.left = avatar.right
            message.anchors.leftMargin = appTheme.narrowSpacing
            message.color = appTheme.leftBubbleColor
            corner.anchors.left = message.left
            msgText.color = appTheme.leftBubbleTextColor

            // 时间
            timeText.anchors.right = message.right

            // 加载动画
            sendStateIcon.anchors.left = message.right
            sendStateIcon.anchors.leftMargin = appTheme.tinySpacing
        } else {
            avatar.anchors.right = bubbleItem.right
            avatar.anchors.rightMargin = appTheme.stdSpacing

            message.anchors.right = avatar.left
            message.anchors.rightMargin = appTheme.narrowSpacing
            message.color = appTheme.rightBubbleColor
            corner.anchors.right = message.right
            msgText.color = appTheme.rightBubbleTextColor

            name.horizontalAlignment = Text.AlignRight

            timeText.anchors.left = message.left

            sendStateIcon.anchors.right = message.left
            sendStateIcon.anchors.rightMargin = appTheme.tinySpacing
        }

        // 名称
        if(_sender !== ChatView.Other) {
            name.visible = false;
        } else {
            name.height = avatar.height / 2
            name.anchors.left = message.left
            name.width = message.width
            name.anchors.bottom = message.top
        }

        timeText.anchors.top = message.bottom
        timeText.anchors.topMargin = appTheme.tinySpacing
        sendStateIcon.anchors.verticalCenter = message.verticalCenter

        bubbleItem.height = timeText.y + appTheme.largeSpacing
    }
}
