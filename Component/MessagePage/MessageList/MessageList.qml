import QtQuick 2.14
import QtQuick.Controls 2.3
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

    AdvancedList {
        id: msgListView
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        //clip: true
        focus: true
        currentIndex: -1
        releaseString: qsTr("释放刷新")

        // 消息项的右键菜单
        StyleMenu {
            id: itemMenu

            // 传入的消息item对象
            property var msgItem: null

            // 当要显示时（未完全打开）
            onAboutToShow: {
                // 如果选中当前项
                if (listModel.getRow(msgItem) === msgListView.currentIndex){
                    readFlagAction.enabled = false
                    deselectAction.enabled = true
                } else {
                    readFlagAction.enabled = true
                    deselectAction.enabled = false
                }

                readFlagAction.text = msgItem.readFlag ? qsTr("标为未读") : qsTr(
                                                             "标为已读")
                topAction.text = msgItem.chatObject.isTop ? qsTr("取消置顶") : qsTr(
                                                                "置顶")
            }
            Action {
                id: readFlagAction
                onTriggered: itemMenu.msgItem.readFlag = !itemMenu.msgItem.readFlag
            }
            Action {
                id: topAction
                onTriggered: {
                    // 先保存当前选中的item
                    listModel.setCurrentSelectedIndex(msgListView.currentIndex)

                    // 设置是否置顶
                    listModel.setMessageTop(itemMenu.msgItem,
                                            !itemMenu.msgItem.chatObject.isTop)
                }
            }
            Action {
                id: deselectAction
                text: qsTr("取消选择")
                onTriggered: {
                    msgListView.currentIndex = -1
                    listModel.setCurrentSelectedIndex(null)
                    itemClicked(null)
                }
            }
        }

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
            property bool isTop: model.msgObject.chatObject.isTop

            id: msgItemRect
            height: 70
            width: msgListView.width
            z: isCurrentItem ? 10 : 1
            layer.enabled: isCurrentItem
            layer.effect: DropShadow {
                radius: 30.0
                samples: 17
                color: isCurrentItem ? appTheme.shadowColor : "transparent"
                verticalOffset: 6
            }
            onIsTopChanged: {
                msgListView.currentIndex = listModel.getCurrentSelectedIndex()

                color = isTop ? appTheme.widgetColor : appTheme.backgroundColor
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
                        else if (msgItem.chatObject.roleType === ChatObject.LAN
                                 && msgItem.chatObject.nickName === '')
                            return msgItem.chatObject.hostAddress
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
                    y: messageText.y
                    anchors.right: parent.right
                    text: (msgItem.unreadMsgCount > 99) ? "99+" : msgItem.unreadMsgCount
                    visible: !msgItem.readFlag
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: true

                onPressed: parent.color = appTheme.widgetColor
                onExited: if (!msgItem.chatObject.isTop)
                              parent.color = appTheme.backgroundColor
                onReleased: if (!msgItem.chatObject.isTop)
                                parent.color = appTheme.backgroundColor

                onClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        if (msgListView.currentIndex !== index) {
                            msgListView.currentIndex = index
                            msgItem.readFlag = true
                            itemClicked(msgItem.chatObject)
                        }
                    } else {
                        itemMenu.msgItem = msgItem
                        itemMenu.popup()
                    }
                }

                // 支持长按弹出菜单
                onPressAndHold: {
                    if (!msgItem.chatObject.isTop)
                        parent.color = appTheme.backgroundColor

                    itemMenu.msgItem = msgItem
                    itemMenu.popup()
                }
            }
        }
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
        z: 30
    }

    Item {
        id: toolBar
        width: parent.width
        anchors.top: title.bottom
        height: searchBox.height + 2 * appTheme.stdSpacing
        z: 31

        InputBox {
            id: searchBox
            placeholderText: qsTr("搜索...")
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: toolBtn.left
                leftMargin: appTheme.stdSpacing
                rightMargin: appTheme.tinySpacing
            }
            iconSource: "qrc:/icons/lightTheme/24x24/search.png"
        }

        StyleButton {
            id: toolBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
            width: appTheme.stdWidgetHeight
            height: appTheme.stdWidgetHeight
        }
    }

    /*StyleButton {
        id: backToTop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: appTheme.wideSpacing
        width: 40
        height: 40
        radius: 20
        normalColor: appTheme.primaryColor1
        pressedColor: appTheme.primaryActiveColor1
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }
    }*/
}
