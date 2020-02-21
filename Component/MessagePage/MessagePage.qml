import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/MessagePage/MessageList"

Item {

    // 聊天视图加载器
    Item {
        id: chatContains
        height: parent.height
        anchors.left: ml_cp_line.right
        anchors.right: parent.right

        property var chatPages: new Object

        Timer {
            id: showPageTimer
            interval: 10
            repeat: false
            running: false

            property var chatObject

            onTriggered: {
                if (chatObject === "")
                    return

                if (!(chatObject in chatContains.chatPages)) {
                    var component = Qt.createComponent(
                                "qrc:/MessagePage/ChatPage/ChatPage.qml")

                    if (component.status === Component.Ready) {
                        var page = component.createObject(chatContains)
                        page.anchors.fill = chatContains
                        chatContains.chatPages[chatObject] = page
                        page.loadChatRecord(chatObject)
                    }
                }

                for (var key in chatContains.chatPages) {
                    chatContains.chatPages[key].visible = false
                }

                chatContains.chatPages[chatObject].visible = true
            }
        }
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
            showPageTimer.chatObject = chatObject
            showPageTimer.start()
        }
    }
}
