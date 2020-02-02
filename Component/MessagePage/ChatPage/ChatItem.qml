import QtQuick 2.14
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
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

    property int _readWidth

    onWidthChanged: _readWidth = Math.min(width * 0.7, msgText.implicitWidth);

    Rectangle {
        id: message
        radius: appTheme.bigRadius
        width: _readWidth
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
            text: _message
            readOnly: true
            selectByMouse: true
            padding: appTheme.stdSpacing
            wrapMode: Text.WordWrap
            font.pixelSize: appTheme.stdTextSize
            selectedTextColor: appTheme.backgroundColor
            selectionColor: appTheme.primaryActiveColor1
            width: _readWidth + 1
        }

        onHeightChanged: {
            bubbleItem.height = timeText.y + appTheme.largeSpacing;
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
        avatar.y = appTheme.stdSpacing;
        message.anchors.top = avatar.verticalCenter;

        if(_sender !== ChatView.Me) {
            // 头像
            avatar.x = appTheme.stdSpacing;

            // 消息
            message.anchors.left = avatar.right
            message.anchors.leftMargin = appTheme.narrowSpacing;
            message.color = appTheme.leftBubbleColor;
            corner.anchors.left = message.left;
            msgText.color = appTheme.leftBubbleTextColor;

            timeText.horizontalAlignment = Text.AlignRight;

            // 加载动画
            sendStateIcon.anchors.left = message.right;
            sendStateIcon.anchors.leftMargin = appTheme.tinySpacing;
        } else {
            avatar.anchors.right = bubbleItem.right;
            avatar.anchors.rightMargin = appTheme.stdSpacing;

            message.anchors.right = avatar.left;
            message.anchors.rightMargin = appTheme.narrowSpacing;
            message.color = appTheme.rightBubbleColor;
            corner.anchors.right = message.right;
            msgText.color = appTheme.rightBubbleTextColor;

            name.horizontalAlignment = Text.AlignRight;

            sendStateIcon.anchors.right = message.left;
            sendStateIcon.anchors.rightMargin = appTheme.tinySpacing;
        }

        // 名称
        if(_sender !== ChatView.Other) {
            name.visible = false;
        } else {
            name.anchors.left = message.left;
            name.anchors.right = message.right;
            name.height = avatar.height / 2;
        }

        // 时间
        timeText.anchors.left = message.left;
        timeText.anchors.right = message.right;
        timeText.anchors.top = message.bottom;
        timeText.anchors.topMargin = appTheme.tinySpacing;

        corner.y = 0;
        sendStateIcon.anchors.verticalCenter = message.verticalCenter;

        bubbleItem.height = timeText.y + appTheme.largeSpacing;
        _readWidth = Math.min(bubbleItem.width * 0.7, msgText.implicitWidth);
    }
}
