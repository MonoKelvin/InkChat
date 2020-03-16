import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.14
import ChatObject 1.0
import ChatItem 1.0
import "qrc:/Element/"

Item {
    id: root

    // 聊天内容，由外界传入，不允许为null
    property var content: null

    // C++中对应的IChatItem对象
    readonly property var chatItem: model.chatItem

    // 是否显示名称。虽然是数值类型，但尽量只用0和1表示显示或隐藏
    property alias showNickName: nameText.opacity

    // 内容的最大宽度
    property int contentMaxWidth

    width: parent.width
    height: timeText.y + appTheme.largeSpacing + timeText.implicitHeight
    onWidthChanged: contentMaxWidth = width * 0.7

    RowLayout {
        id: mainLayout
        spacing: appTheme.narrowSpacing
        anchors.fill: parent
        anchors.margins: appTheme.stdSpacing

        Avatar {
            id: avatar
            Layout.maximumWidth: width
            Layout.maximumHeight: height
            imageSource: chatItem.sender.avatar
            Layout.alignment: Qt.AlignTop
        }

        Item {
            id: layoutContainer
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                id: columnLayout
                spacing: appTheme.tinySpacing
                width: content.width
                height: parent.height

                Text {
                    id: nameText
                    opacity: 0
                    font.pixelSize: appTheme.stdTextSize
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    height: avatar.height / 2
                    width: parent.width
                    text: chatItem.sender.nickName
                }

                Item {
                    id: contentContainer
                    width: parent.width
                    height: content.height
                    clip: true

                    // todo
                    Rectangle {
                        id: sendStateIcon
                        width: 12
                        height: 12
                        color: "red"
                        anchors.margins: appTheme.tinySpacing
                    }
                }

                Text {
                    id: timeText
                    color: appTheme.subTextColor
                    font.pixelSize: appTheme.smallTextSize
                    width: parent.width
                    text: chatItem.time
                }
            }
        }
    }

    Component.onCompleted: {
        contentContainer.children.push(content)
        sendStateIcon.anchors.verticalCenter = content.verticalCenter

        // 如果是“我”就显示在右边，否则显示在左边
        if (chatItem.sender.roleType === ChatObject.Me) {
            mainLayout.layoutDirection = Qt.RightToLeft
            columnLayout.anchors.right = layoutContainer.right
            nameText.horizontalAlignment = Text.AlignRight
            timeText.horizontalAlignment = Text.AlignLeft
            sendStateIcon.anchors.right = content.left
        } else {
            nameText.horizontalAlignment = Text.AlignLeft
            timeText.horizontalAlignment = Text.AlignRight
            sendStateIcon.anchors.left = content.right
        }

        root.contentMaxWidth = width * 0.7
    }
}
