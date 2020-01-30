import QtQuick 2.7
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import User 1.0
import "qrc:/Element/"
import "qrc:/Navigation/"
import "qrc:/MessagePage/MessageList"

ApplicationWindow {
    id: window
    title: "InkChat"
    width: 1080
    height: 640
    visible: true
    opacity: 1
    color: appTheme.backgroundColor

    minimumWidth: 800
    minimumHeight: 512

    Component.onCompleted: {
        messageList.refresh([
                                {
                                   id: 1,
                                   name: "Tnoy Stack",
                                   message: "昨天发来的100多条消息。",
                                   loginState: User.Online,
                                   unreadMessageNumber: 10,
                                   read: false,
                                   messageTime: "13:16"
                                },
                                {
                                    id: 2,
                                    name: "Tnoy SAlita",
                                    message: "昨天发来lo! how are you?",
                                    loginState: User.Offline,
                                    unreadMessageNumber: 120,
                                    read: false,
                                    messageTime: "10:16"
                                },
                                {
                                    id: 3,
                                    name: "Francis King",
                                    message: "昨天发来的100多条消息。",
                                    loginState: User.Busy,
                                    unreadMessageNumber: 0,
                                    read: true,
                                    messageTime: "13:16"
                                },
                                {
                                    id: 3,
                                    name: "Jack Lloyd",
                                    message: "1小时前已读的消息。",
                                    loginState: User.NoneState,
                                    unreadMessageNumber: 100,
                                    read: false,
                                    messageTime: "13:16"
                                },
                            ]);
    }

    AppTheme{
        id: appTheme
    }

    Navigation {
        id: navigation

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }

        onNavigate: function(i){console.log(i)}
    }

    MessageList {
        id: messageList
        anchors.left: navigation.right
        height: window.height
        width: 270
    }
}
