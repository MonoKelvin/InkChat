import QtQuick 2.14
import User 1.0

Item {
    id: groupItem
    height: 32
    clip: true

    Behavior on height { NumberAnimation { easing.type: Easing.InOutQuad; duration: 400 } }

    Component.onCompleted: {
        for ( var index = 0; index < _members.count; ++index ) {
            var buf = _members.get(index);

            var data = {};
            data._type = buf["type"];
            data._id = buf["id"];
            data._avatar = buf["avatar"];
            data._onlineState = buf["onlineState"];
            data._name = buf["name"];
            data._signature = buf["signature"];

            if(data._type === User.Stranger) {
                data._friendCount = buf["friendCount"];
                data._onlineDays = buf["onlineDays"];
            }

            friendListModel.append(data);
        }

        friendListView.height = _members.count * 90;
        friendListView.y = -1 * friendListView.height;
    }

    Item {
        x: 0
        y: groupNameText.height + groupNameText.padding
        width: parent.width
        height: friendListView.height

        ListView {
            id: friendListView
            x: 0; y: 0
            width: parent.width
            height: 0
            spacing: appTheme.tinySpacing
            visible: y !== (-1 * friendListView.height)
            boundsBehavior: Flickable.StopAtBounds

            model: ListModel {
                id: friendListModel
            }

            delegate: Loader {
                width: friendListView.width
                height: 80
                source: "FriendListItem.qml"
            }
        }
    }

    Text {
        id: groupNameText
        text: _groupName
        width: parent.width
        height: 32
        padding: appTheme.stdSpacing
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.subTextColor

        Behavior on color { ColorAnimation { duration: 200 } }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                if (friendListModel.count) {
                    if (friendListView.visible) {
                        friendListView.y = -1 * friendListView.height;
                        groupItem.height = 32;
                    } else {
                        friendListView.y = 0;
                        groupItem.height = 32 + friendListView.height;
                    }
                }
            }
        }
    }
}

