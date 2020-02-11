import QtQuick 2.12
import QtGraphicalEffects 1.0
import ChatPageManage 1.0
import "qrc:/Element/"

Rectangle {
    property alias titleName: titleNameText.text

    color: appTheme.backgroundColor
    titleName: qsTr("聊天")

    ListView {
        id: chatListView
        width: parent.width
        anchors.fill: parent
        topMargin: titleBar.height

        model: ListModel {
            id: chatListModel
        }
        delegate: Loader {
            width: chatListView.width
            source: "ChatItem.qml"
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

                }
            }
        }
        footerPositioning: ListView.OverlayFooter

        onContentYChanged: {
            if (contentY > -titleBar.height)
                blurEffect.update()
        }
    }

    Text {
        id: nochatText
        text: qsTr("暂时没有聊天消息...")
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.subTextColor
        visible: false
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


    /**
     * 设置为空视图
     */
    function setVoidView() {
        chatListView.visible = false
        // chatListModel.clear();
        nochatText.visible = true
        titleName = qsTr("聊天")
    }

    function refresh(items) {
        chatListModel.clear()
        for (var index = 0; index < items.length; index++) {
            var buf = items[index]
            chatListModel.append({
                                     "_avatar": buf["avatar"] ? buf["avatar"] : "",
                                     "_messageType": buf["messageType"],
                                     "_name": buf["name"],
                                     "_message": buf["message"],
                                     "_sendTime": buf["sendTime"],
                                     "_sendState": buf["sendState"] ? buf["sendState"] : ChatPageManage.Sending,
                                     "_sender": buf["sender"]
                                 })
        }
    }
}
