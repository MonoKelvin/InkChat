import QtQuick 2.0

Item {
    id: friendListItem
    width: friendListView.width
    height: 80
    clip: true

    Rectangle {
        id: friendCard
        anchors.fill: parent
        radius: 10
        color: "#ffffff"

        // TODO: use image to replace it
        Rectangle {
            id: avatar
            height: 40
            width: 40
            radius: 20
            color: "lightgray"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 25

            Component.onCompleted: setOnlineState(onlineState)

        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: avatar.right
            anchors.leftMargin: 15
            anchors.right: idTextContainer.left
            anchors.rightMargin: 25
            spacing: 5

            Text {
                font.bold: true
                font.pixelSize: 14
                color: "#3b4151"
                text: nickName
                width: parent.width
                elide: Text.ElideRight
            }
            Text {
                font.pixelSize: 12
                color: "#a8adbd"
                text: signature
                width: parent.width
                elide: Text.ElideRight
            }
        }
        Rectangle {
            id: idTextContainer
            color: "#ffedd5"
            radius: 6
            height: 24
            width: idText.contentWidth + 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25

            Text {
                id: idText
                font.pixelSize: 12
                anchors.centerIn: parent
                color: "#feaa2a"
                text: "id:" + id
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: console.log("item clicked")
    }

    function setOnlineState(state) {
        if (state !== User.NoneState && component.status === Component.Ready) {
            var osRect = Qt.createQmlObject('import QtQuick 2.0;Rectangle {
                                                width: 10;
                                                height: 10;
                                                radius: 5;
                                                anchors: {
                                                    left: avatar.left;
                                                    right:avatar.right;
                                                }
                                            }', avatar, "qmlobj");
            switch(state) {
            case User.Online:
                osRect.color = "#2afe75";
                break;
            case User.Offline:
                osRect.color = "#a8adbd";
                break;
            case User.Busy:
                osRect.color = "#feaa2a";
                break;
            default:
                delete osRect;
                break;
            }
        }
    }
}
