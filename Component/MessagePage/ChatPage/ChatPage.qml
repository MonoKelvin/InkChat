﻿import QtQuick 2.0
import "qrc:/Element/"
import ChatView 1.0

Item {
    property alias titleName: titleNameText.text

    Component.onCompleted: {

    }

    ListView {
        id: chatListView
        anchors.fill: parent

        model: ListModel {
            id: chatListModel
        }
        delegate: Loader {
            id:loader
            width: chatListView.width
            source: "ChatItem.qml"
        }
        header: Item {
            height: titleBar.height + appTheme.stdSpacing
            width: chatListView.width
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

    Rectangle {
        id: titleBar
        width: parent.width
        height: 55
        color: appTheme.backgroundColor

        Text {
            id: titleNameText
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.right: menuIconBtn.left
            anchors.rightMargin: appTheme.stdSpacing
            font.pixelSize: appTheme.titleTextSize
            font.bold: true
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
        chatListView.visible = false;
        //        chatListModel.clear();
        nochatText.visible = true;
        titleName = qsTr("聊天");
    }

    function refresh(items) {
        chatListModel.clear();
        for (var index = 0; index < items.length; index++){
            var buf = items[index];
            chatListModel.append({
                                     _avatar: buf["avatar"]?buf["avatar"]:"",
                                     _messageType: buf["messageType"],
                                     _name: buf["name"],
                                     _message: buf["message"],
                                     _sendTime: buf["sendTime"],
                                     _sendState: buf["sendState"]?buf["sendState"]:ChatView.Sending,
                                     _sender: buf["sender"],
                                 });
        }
    }
}
