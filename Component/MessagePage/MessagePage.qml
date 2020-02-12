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
                              "message": "this is a messagethis is a messagethis is a messagethis is a messagethis is a messagethis is a message",
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
