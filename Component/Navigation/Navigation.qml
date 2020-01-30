import QtQuick 2.1
import QtQuick.Controls 1.4
import "qrc:/Element/"

Rectangle {
    width: 70
    color: appTheme.backgroundColor

    signal navigate(var index)

    Column {
        id: navigation
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        property int currentIndex: 0

        ExclusiveGroup {id : exclusiveGroup}

        Component.onCompleted: {
            btnMessage.checked = true;
        }

        IconButton {
            id: btnMessage
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: checked?navigate(0):null
        }
        IconButton {
            id: btnFriend
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: checked?navigate(1):null
        }
        IconButton {
            id: btnMine
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: checked?navigate(2):null
        }
        IconButton {
            id: btnSettings
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: checked?navigate(3):null
        }
    }
}
