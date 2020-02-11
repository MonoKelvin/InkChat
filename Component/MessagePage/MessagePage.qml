import QtQuick 2.0
import QtGraphicalEffects 1.0
import User 1.0
import ChatPageManage 1.0
import "qrc:/Element/"
import "qrc:/MessagePage/ChatPage"
import "qrc:/MessagePage/MessageList"

Item {


    /**
     * @berif 显示聊天视图
     * @param 传入的聊天对象，可以是群聊或单个用户
     */
    function showChatView(chatObj) {}

    Component.onCompleted: {

        messageList.refresh([{
                                 "id": 1,
                                 "type": User.Group,
                                 "avatar": "",
                                 "name": "Tnoy Stack",
                                 "message": "昨天发来的100多条消息。",
                                 "loginState": User.Online,
                                 "unreadMessageNumber": 10,
                                 "read": false,
                                 "messageTime": "昨天"
                             }, {
                                 "id": 2,
                                 "type": User.Friend,
                                 "avatar": "",
                                 "name": "Tnoy SAlita",
                                 "message": "昨天发来lo! how are you?",
                                 "loginState": User.Offline,
                                 "unreadMessageNumber": 120,
                                 "read": false,
                                 "messageTime": "10:16"
                             }, {
                                 "id": 3,
                                 "type": User.LAN,
                                 "avatar": "",
                                 "name": "局域网",
                                 "message": "昨天发来的100多条消息。",
                                 "loginState": User.NoneState,
                                 "unreadMessageNumber": 0,
                                 "read": true,
                                 "messageTime": "13:16"
                             }, {
                                 "id": 3,
                                 "avatar": "",
                                 "name": "Jack Lloyd",
                                 "message": "1小时前已读的消息。",
                                 "loginState": User.NoneState,
                                 "unreadMessageNumber": 100,
                                 "read": false,
                                 "messageTime": "13:16"
                             }, {
                                 "id": 1,
                                 "avatar": "",
                                 "name": "Tnoy Stack",
                                 "message": "昨天发来的100多条消息。",
                                 "loginState": User.Online,
                                 "unreadMessageNumber": 10,
                                 "read": false,
                                 "messageTime": "13:16"
                             }])

        const t = new Date()
        const curTime = t.getHours() + ":" + t.getMinutes()

        chatPage.refresh([{
                              "avatar": null,
                              "messageType": ChatPageManage.Message,
                              "sender": ChatPageManage.Ta,
                              "name": "Tnoy Stack",
                              "message": "this is a message",
                              "sendTime": curTime
                          }, {
                              "avatar": null,
                              "messageType": ChatPageManage.Message,
                              "sender": ChatPageManage.Me,
                              "name": "Tnoy Stack",
                              "message": "this is a message",
                              "sendTime": curTime
                          }, {
                              "avatar": null,
                              "messageType": ChatPageManage.Message,
                              "sender": ChatPageManage.Ta,
                              "name": "Tnoy Stack",
                              "message": "this is a message",
                              "sendTime": curTime
                          }, {
                              "avatar": null,
                              "messageType": ChatPageManage.Message,
                              "sender": ChatPageManage.Me,
                              "name": "Tnoy Stack",
                              "message": "this is a messaQML debugging is enabled. Only QML debugging is enabled. Only use this in a safe environment.use this in a safe environment.ge",
                              "sendTime": curTime
                          }])
    }

    ChatPage {
        id: chatPage
        height: parent.height
        anchors.left: ml_cp_line.right
        anchors.right: parent.right
        titleName: qsTr("聊天")
    }

    // 分割线
    Rectangle {
        id: ml_cp_line
        height: parent.height
        width: 2
        color: appTheme.borderLineColor
        anchors.left: messageList.right
    }

    MessageList {
        id: messageList
        width: 300
        height: parent.height
        anchors.left: parent.left

        // layer.enabled: true
        // layer.effect: DropShadow {
        //     radius: 20.0
        //     samples: 17
        //     color: appTheme.shadowColor
        //     horizontalOffset: 1
        // }
        onItemClicked: {
            console.log(msgId)
        }
    }
}
