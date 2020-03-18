import QtQuick 2.12
import QtGraphicalEffects 1.0
import ChatObject 1.0
import ChatListModel 1.0
import ChatItem 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as Utility

Rectangle {
    color: appTheme.backgroundColor

    // 加载聊天视图，参数为聊天对象
    function loadChatRecord(chatObject) {
        titleNameText.text = chatObject.nickName

        switch(chatObject.roleType) {
        case ChatObject.LAN:
            flagBadge.text = qsTr("局域网")
            break
        case ChatObject.Friend:
            flagBadge.text = qsTr("好友")
            break
        }

        chatListModel.initLoad(chatObject)

        // 加载完立刻滚动到底部
        chatListView.positionViewAtEnd()
    }

    AdvancedList {
        id: chatListView
        anchors.fill: parent
        topMargin: titleBar.height

        // 未加载前的数量，便于加载后定位
        property int _oldCount: 0

        onLoading: {
            _oldCount = count
            chatListModel.fetchMore()
            loadState = AdvancedList.Loaded
        }

        onLoaded: {
            positionViewAtIndex(count - _oldCount, ListView.Beginning)
            if (contentY > 0)
                contentY -= titleBar.height + appTheme.stdSpacing
        }

        model: ChatListModel {
            id: chatListModel

            onFailed: {
                Utility.createToast(msg, window)
            }

            onChatAdded: {
                // 如果用户试图将列表滚动到上方，则接收消息但不滚动到底部，除非是我发送的
                if(!isMe && chatListView.contentY < chatListView.contentHeight
                   - chatListView.height) {
                    return
                }

                // 滚动到底部
                chatListView.positionViewAtEnd()
            }
        }

        // 消息代理
        delegate: Loader {
            width: chatListView.width
            source: model.chatItem.qmlFile()
            Component.onCompleted: {
                // 只有多人聊天才显示名称
                if(chatListModel.chatObject.roleType & ChatObject.MultiPerson)
                    item.showNickName = 1
            }
        }

        // 输入框
        footer: Loader {
            id: inputer
            z: 10
            width: chatListView.width
            source: "ChatInputer.qml"

            // 接收来自输入框的信号
            Connections {
                target: inputer.item
                onSendChat: {
                    chatListModel.sendChat(content)
                }
            }

            // 防止鼠标事件穿透
            MouseArea {
                anchors.fill: parent
                onWheel: {

                    // nothing to do.
                }
            }
        }
        footerPositioning: ListView.OverlayFooter

        onContentYChanged: {
            if (contentY > -titleBar.height)
                blurEffect.update()
        }
    }

    // 分割线
    Rectangle {
        id: nsv_page_line
        width: parent.width
        height: 2
        color: appTheme.borderLineColor
        anchors.top: titleBar.bottom
    }

    Rectangle {
        id: titleBar
        width: parent.width
        height: 55
        color: appTheme.backgroundColor

        // layer.enabled: true
        // layer.effect: DropShadow {
        //     radius: 20.0
        //     samples: 17
        //     color: appTheme.shadowColor
        //     verticalOffset: 1
        // }
        BlurEffect {
            id: blurEffect
            target: titleBar
            background: chatListView
            blurRadius: 72
        }

        Text {
            id: titleNameText
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
            font.pixelSize: appTheme.titleTextSize
        }

        Badge {
            id: flagBadge
            color: appTheme.subColor2
            contentText.color: appTheme.primaryColor2
            contentText.leftPadding: appTheme.tinySpacing
            contentText.rightPadding: appTheme.tinySpacing
            radius: appTheme.stdRadius
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: titleNameText.right
            anchors.leftMargin: appTheme.narrowSpacing
            height: implicitHeight + 4
        }

        StyleButton {
            id: menuIconBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
            icon.name: "menu"
        }
    }
}
