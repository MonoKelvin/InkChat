import QtQuick 2.14
import QtGraphicalEffects 1.0
import MessageListModel 1.0
import ChatObject 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as Utility

Rectangle {
    id: messageList
    color: appTheme.backgroundColor

    signal itemClicked(var chatObject)

    Component.onCompleted: {
        listModel.load()
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
        }
    }

    AdvancedList {
        id: msgListView
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        clip: true
        focus: true
        currentIndex: -1

        onLoading: {
            var i = 0
            while (i++ < 100000000)
                i++

            loadState = AdvancedList.Loaded
        }

        model: MessageListModel {
            id: listModel
            onFailed: function (msg) {
                Utility.createToast(msg, window)
            }
        }

        delegate: Rectangle {
            property var msgItem: model.msgObject
            property bool isCurrentItem: msgListView.currentIndex === index

            id: msgItemRect
            height: 70
            width: msgListView.width
            color: {
                if (msgItem.chatObject.isTop)
                    return appTheme.widgetColor
                else
                    return appTheme.backgroundColor
            }
            z: isCurrentItem ? 10 : 1
            layer.enabled: isCurrentItem
            layer.effect: DropShadow {
                radius: 30.0
                samples: 17
                color: isCurrentItem ? appTheme.shadowColor : "transparent"
                verticalOffset: 6
            }

            function onReadFlagChanged() {
                if (msgItem.readFlag) {
                    msgBadge.visible = false
                } else {
                    if (msgItem.unreadMsgCount <= 0) {
                        return
                    }

                    msgBadge.visible = true
                }
            }

            function onUnreadMsgCountChanged() {
                if (msgItem.readFlag)
                    return

                if (msgItem.unreadMsgCount <= 0) {
                    msgBadge.width = 0
                    msgBadge.visible = false
                    return
                }

                msgBadge.visible = true
                if (msgItem.unreadMsgCount <= 99) {
                    msgBadge.text = msgItem.unreadMsgCount
                } else {
                    msgBadge.text = "99+"
                }
                msgBadge.width = msgBadge.contentWidth + 14
            }

            Component.onCompleted: {
                msgItem.readFlagChanged.connect(onReadFlagChanged)
                msgItem.unreadMsgCountChanged.connect(onUnreadMsgCountChanged)
            }

            Avatar {
                id: avatar
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: appTheme.stdSpacing
                onlineState: msgItem.chatObject.onlineState
                imageSource: msgItem.chatObject.avatar

                onOnlineStateChanged: {
                    switch (onlineState) {
                    case ChatObject.Online:
                        nameText.color = appTheme.mainTextColor
                        msgBadge.color = appTheme.subColor3
                        msgBadge.textColor = appTheme.primaryColor3
                        break
                    case ChatObject.Offline:
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
                    font.bold: Qt.platform.os == "windows" ? true : false
                    verticalAlignment: Text.AlignBottom
                    font.pixelSize: appTheme.stdTextSize
                    color: appTheme.mainTextColor
                    elide: Text.ElideRight
                    text: {
                        if (msgItem.chatObject.roleType === ChatObject.Friend
                                && msgItem.chatObject.remark !== '')
                            return msgItem.chatObject.remark
                        return msgItem.chatObject.nickName
                    }
                }
                Text {
                    id: messageText
                    width: parent.width
                    font.pixelSize: appTheme.smallTextSize
                    color: appTheme.subTextColor
                    elide: Text.ElideRight
                    text: msgItem.message
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
                    y: nameText.y
                    anchors.right: parent.right
                    text: msgItem.time
                }
                Badge {
                    id: msgBadge
                    visible: true
                    y: messageText.y
                    anchors.right: parent.right
                    text: msgItem.unreadMsgCount

                    Component.onCompleted: {
                        msgItemRect.onReadFlagChanged()
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // 加载聊天记录
                    if (msgListView.currentIndex !== index)
                        messageList.itemClicked(msgItem.chatObject)

                    msgListView.currentIndex = index

                    // 标记已读
                    msgItem.readFlag = true
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
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }
    }
}
