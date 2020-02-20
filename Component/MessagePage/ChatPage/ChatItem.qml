import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.14
import ChatObject 1.0
import ChatItem 1.0
import "qrc:/Element/"

Item {
    id: root

    // 聊天内容，由外界传入
    property var content: null

    // C++中对应的IChatItem对象
    readonly property var chatItem: model.chatItem

    // 内容的真实宽度，其最大宽度不大于该聊天项的0.7倍
    property int contentReadWidth

    onWidthChanged: contentReadWidth = Math.min(width * 0.7,
                                                chatContainer.implicitWidth)

    RowLayout {
        id: chatLayout
        spacing: appTheme.narrowSpacing
        anchors.fill: parent
        anchors.margins: appTheme.stdSpacing

        Avatar {
            id: avatar
            imageSource: chatItem.sender.avatar
        }

        ColumnLayout {
            width: contentReadWidth
            Layout.fillHeight: true
            Layout.leftMargin: appTheme.narrowSpacing
            Layout.rightMargin: appTheme.narrowSpacing

            Text {
                id: name
                Layout.fillWidth: true
                font.pixelSize: appTheme.stdTextSize
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                height: avatar.height / 2
                visible: false
                text: chatItem.sender.nickName
            }

            Item {
                id: chatContainer
                height: content.height
                clip: true

                children: content
            }

            Text {
                id: timeText
                Layout.fillWidth: true
                Layout.topMargin: appTheme.tinySpacing
                text: chatItem.time
                color: appTheme.subTextColor
                font.pixelSize: appTheme.smallTextSize
            }
        }

        Rectangle {
            id: sendStateIcon
            Layout.alignment: Qt.AlignVCenter
            Layout.margins: appTheme.tinySpacing
            width: 12
            height: 12
            color: "red"
        }
    }

    Component.onCompleted: {
        if (content === null)
            return

        // 名称
        if (chatItem.sender.roleType === ChatObject.LAN
                || chatItem.sender.roleType === ChatObject.Group) {
            name.visible = true
        }

        chatLayout.height = timeText.y + appTheme.largeSpacing
    }
}
