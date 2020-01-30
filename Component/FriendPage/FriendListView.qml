import QtQuick 2.0
import User 1.0

ListView {
    id: friendListView
    spacing: 10

    model: ListModel {
        id: friendListModel
    }

    delegate: Loader{ source: "FriendListItem.qml" }

    function refresh( items ) {
        friendListModel.clear();

        for (var index = 0; index < items.length; index++)
        {
            friendListModel.append({
                                       nickName: items[index]["nickName"],
                                       signature: items[index]["signature"],
                                       id: items[index]["id"]
                                   });
        }
    }

}

