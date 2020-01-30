import QtQuick 2.0

Rectangle {
    color: appTheme.backgroundColor

    ListView {
        id: messageListView
        anchors.fill: parent
        clip: true

        model: ListModel{
            id: messageListModel
        }

        delegate: Loader {
            id:loader
            width: messageListView.width
            source: "MessageItem.qml"
        }
    }

    function refresh( items ) {
        messageListModel.clear();

        for (var index = 0; index < items.length; index++){
            var buf = items[index];
            messageListModel.append({
                                        id: buf["id"],
                                        name: buf["name"],
                                        message: buf["message"],
                                        loginState: buf["loginState"],
                                        unreadMessageNumber: buf["unreadMessageNumber"],
                                        read: buf["read"],
                                        messageTime: buf["messageTime"]
                                    });
        }
    }
}
