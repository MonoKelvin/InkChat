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

    // 加载聊天视图，参数为聊天对象
    function loadChatRecord(chatObject) {
        nochatText.visible = false

        titleName = chatObject.nickName
        chatListModel.load(chatObject.id)
    }

    ListView {
        id: chatListView
        width: parent.width
        anchors.fill: parent
        topMargin: titleBar.height

        Text {
            id: nochatText
            text: qsTr("暂时没有聊天消息...")
            anchors.centerIn: parent
            font.pixelSize: appTheme.stdTextSize
            color: appTheme.subTextColor
            visible: true
        }

        model: ChatListModel {
            id: chatListModel

            onFailed: {
                Utility.createToast(msg, window)

                // 加载失败就设置为空的视图
                nochatText.visible = false
                titleName = qsTr("聊天")
            }
        }

        delegate: Loader {
            width: chatListView.width
            source: model.chatItem.qmlFile()
        }

        footer: Loader {
            id: inputer
            z: 10
            width: chatListView.width
            source: "ChatInputer.qml"

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
            blurRadius: 128
        }

        Text {
            id: titleNameText
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.right: menuIconBtn.left
            anchors.rightMargin: appTheme.stdSpacing
            font.pixelSize: appTheme.titleTextSize
        }

        IconButton {
            id: menuIconBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
