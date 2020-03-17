import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/MessagePage/MessageList"
import "qrc:/js/js/Utility.js" as Utility

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
                // 取消其他页面的可见性
                for (var key in chatContains.chatPages)
                    chatContains.chatPages[key].page.visible = false

                if (chatObject === null)
                    return

                // 如果该页面时没有打开过，就动态加载一份
                if (!(chatObject in chatContains.chatPages)) {
                    var component = Qt.createComponent("qrc:/MessagePage/ChatPage/ChatPage.qml")
                    if (component.status === Component.Ready) {
                        var page = component.createObject(chatContains)
                        page.anchors.fill = chatContains

                        chatContains.chatPages[chatObject] = {
                            "page": page,
                            "time": Utility.currentTimeNumber()
                        }

                        page.loadChatRecord(chatObject)
                    }
                }

                // 更新最近使用时间
                chatContains.chatPages[chatObject].time = Utility.currentTimeNumber()
                // 设为可见
                chatContains.chatPages[chatObject].page.visible = true
            }
        }

        // 用于定时销毁聊天页面的计时器，节约系统资源
        Timer {
            id: destroyPageTimer

            // 每3分钟检查一遍
            interval: 180000
            repeat: true
            running: true

            // 使用最近未使用算法销毁页面
            onTriggered: {
                // 最小时间，表示最早使用的页面，初始化为无限大
                var minTime = Infinity
                // 页面总数
                var count = 0

                for (var key in chatContains.chatPages) {
                    count++

                    // 选择最早使用的页面
                    if (chatContains.chatPages[key].time < minTime)
                        minTime = chatContains.chatPages[key].time
                }

                // 超过6个页面就释放资源
                if (count > 6) {
                    for (var i in chatContains.chatPages) {
                        // 销毁最早使用的页面
                        if (minTime === chatContains.chatPages[i].time) {
                            chatContains.chatPages[i].page.destroy()
                            delete chatContains.chatPages[i]
                            break
                        }
                    }
                }
            }
        }

        Component.onCompleted: destroyPageTimer.start()
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
        width: 280
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
