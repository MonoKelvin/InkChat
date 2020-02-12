import QtQuick 2.14
import QtGraphicalEffects 1.0
import MessageListModel 1.0
import MessageItem 1.0
import User 1.0
import "qrc:/Element/"

Rectangle {
    id: messageList
    color: appTheme.backgroundColor

    signal itemClicked(var msgId)

    Component.onCompleted: {
        model.load("D:/GraduationProject/build-InkChatQml-Desktop_Qt_5_14_0_MinGW_64_bit-Debug/data/1/cache/message")
    }

    Text {
        id: title
        text: qsTr("消息列表")
        width: parent.width
        height: 45
        anchors.top: parent.top
        padding: appTheme.wideSpacing
        horizontalAlignment: Text.AlignHCenter
        color: appTheme.mainTextColor
    }

    Item {
        id: toolBar
        width: parent.width
        anchors.top: title.bottom
        height: searchBox.height + 2 * appTheme.stdSpacing

        InputBox {
            id: searchBox
            placeholderText: qsTr("搜索...")
            icon: IconButton {
                clickedColor: appTheme.primaryColor1
                checked: true
            }
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: toolBtn.left
                leftMargin: appTheme.stdSpacing
                rightMargin: appTheme.tinySpacing
            }
        }

        IconButton {
            id: toolBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
            width: appTheme.stdWidgetHeight
            height: appTheme.stdWidgetHeight
            checked: true //test
        }
    }

    ListView {
        id: msgListView
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        clip: true
        focus: true

        model: MessageListModel {
            id: model
        }

        delegate: Rectangle {
            height: 70
            width: msgListView.width
            color: (msgListView.currentIndex === index) ? appTheme.widgetColor : "transparent"

            MessageItem {
                id: messageItem
                onReadFlagChanged: {
                    if (readFlag) {
                        msgBadge.width = 0
                        msgBadge.visible = false
                    } else {
                        if (unreadMsgCount <= 0) {
                            return
                        }

                        msgBadge.visible = true
                        msgBadge.width = msgBadge.contentWidth + 10
                    }
                }

                onUnreadMsgCountChanged: {
                    if (readFlag)
                        return

                    if (unreadMsgCount <= 0) {
                        msgBadge.width = 0
                        msgBadge.visible = false
                        return
                    }

                    msgBadge.visible = true
                    if (unreadMsgCount <= 99) {
                        msgBadge.text = unreadMsgCount
                    } else {
                        msgBadge.text = "99+"
                    }
                    msgBadge.width = msgBadge.contentWidth + 14
                }
            }

            Avatar {
                id: avatar
                onlineState: messageItem.chatObject.onlineState
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: appTheme.stdSpacing

                onOnlineStateChanged: {
                    switch (onlineState) {
                    case User.Online:
                        nameText.color = appTheme.mainTextColor
                        msgBadge.color = appTheme.subColor3
                        msgBadge.textColor = appTheme.primaryColor3
                        break
                    case User.Offline:
                        nameText.color = appTheme.subTextColor
                        msgBadge.color = appTheme.tintColor
                        msgBadge.textColor = appTheme.subTextColor
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
                    text: messageItem.chatObject.name
                }
                Text {
                    id: messageText
                    width: parent.width
                    font.pixelSize: appTheme.smallTextSize
                    color: appTheme.subTextColor
                    elide: Text.ElideRight
                    text: messageItem.message
                }
            }

            Item {
                id: column_time_badge
                height: column_name_msg.height
                width: Math.max(timeText.implicitWidth, msgBadge.implicitWidth)
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
                    text: messageItem.time
                    y: nameText.y
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Badge {
                    id: msgBadge
                    text: unreadMsgNumber
                    y: messageText.y
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    msgListView.currentIndex = index
                    messageList.itemClicked(msgId)
                }
            }
        }
    }

    IconButton {
        id: backToTop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: appTheme.wideSpacing
        width: 40
        height: 40
        radius: 20
        normalColor: appTheme.primaryColor1
        clickedColor: appTheme.primaryActiveColor1
        layer.enabled: true
        layer.effect: DropShadow {
            //            anchors.fill: navigation
            //            source: navigation
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }
    }
}
