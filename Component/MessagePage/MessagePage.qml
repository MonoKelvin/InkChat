import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/MessagePage/ChatPage"
import "qrc:/MessagePage/MessageList"

Item {
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
            chatPage.loadChatRecord(chatObject)
        }
    }
}
