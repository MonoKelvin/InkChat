import QtQuick 2.12
import QtGraphicalEffects 1.0
import ChatListModel 1.0
import ChatItem 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as Utility

Rectangle {
    property alias titleName: titleNameText.text

    color: appTheme.backgroundColor
    titleName: qsTr("聊天")

    // 私有：当前聊天对象的id
    property int _chatObjId

    // 加载聊天视图，参数为聊天对象
    function loadChatRecord(chatObject) {

        titleName = chatObject.nickName
        _chatObjId = chatObject.id
        chatListModel.load(_chatObjId)

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
            chatListModel.load(_chatObjId)
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
        }

        // 消息代理
        delegate: Loader {
            width: chatListView.width
            source: model.chatItem.qmlFile()
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
                    chatListModel.sendChat(_chatObjId, content)

                    // 滚动到底部
                    chatListView.positionViewAtEnd()
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
            blurRadius: 64
        }

        Text {
            id: titleNameText
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
            font.pixelSize: appTheme.titleTextSize
        }

        StyleButton {
            id: menuIconBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
