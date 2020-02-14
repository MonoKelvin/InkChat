import QtQuick 2.14
import ChatObject 1.0

Item {
    id: groupItem
    height: 32
    clip: true

    Behavior on height {
        NumberAnimation {
            easing.type: Easing.InOutQuad
            duration: 350
        }
    }

    Component.onCompleted: {
        for (var index = 0; index < _members.count; ++index) {
            var buf = _members.get(index)

            var data = {}
            data._type = buf["type"]
            data._id = buf["id"]
            data._avatar = buf["avatar"]
            data._onlineState = buf["onlineState"]
            data._name = buf["name"]
            data._signature = buf["signature"]

            if (data._type === ChatObject.Stranger) {
                data._friendCount = buf["friendCount"]
                data._onlineDays = buf["onlineDays"]
            }

            friendListModel.append(data)
        }

        friendListView.height = _members.count * 85
        friendListView.y = -friendListView.height
    }

    Item {
        x: 0
        y: 32
        width: parent.width
        height: friendListView.height
        clip: true

        ListView {
            id: friendListView
            x: 0
            y: 0
            width: parent.width
            height: 0
            spacing: appTheme.tinySpacing
            visible: y !== -friendListView.height
            boundsBehavior: Flickable.StopAtBounds

            model: ListModel {
                id: friendListModel
            }

            Behavior on y {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 350
                }
            }

            delegate: Loader {
                width: friendListView.width
                height: 80
                source: "FriendListItem.qml"
            }
        }
    }

    // 组名
    Text {
        id: groupNameText
        width: parent.width
        height: 32
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.subTextColor
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        text: _groupName

        // 组成员数量badge
        Rectangle {
            radius: appTheme.stdRadius
            x: groupNameText.contentWidth + appTheme.narrowSpacing
            anchors.verticalCenter: parent.verticalCenter
            width: memberCountText.implicitWidth
            height: appTheme.stdBadgeHeight
            color: appTheme.subColor1

            Text {
                id: memberCountText
                height: parent.height
                font.pixelSize: appTheme.smallTextSize
                color: appTheme.primaryColor1
                verticalAlignment: Text.AlignVCenter
                padding: appTheme.tinySpacing
                text: friendListView.count + "/" + _groupMaxCount
            }
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                if (friendListModel.count) {
                    if (friendListView.visible) {
                        friendListView.y = -friendListView.height
                        groupItem.height = 32
                    } else {
                        friendListView.y = 0
                        groupItem.height = 32 + friendListView.height
                    }
                }
            }
        }
    }
}
