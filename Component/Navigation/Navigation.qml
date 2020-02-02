import QtQuick 2.1
import QtQuick.Controls 1.4
import "qrc:/Element/"

Rectangle {
    width: 70
    color: appTheme.backgroundColor

    signal navigate(var index)

    property int currentIndex: 0

    onCurrentIndexChanged: navigate(currentIndex);

    Column {
        id: navigation
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

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
            onCheckedChanged: if(checked) currentIndex = 0;
        }
        IconButton {
            id: btnFriend
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: if(checked) currentIndex = 1;
        }
        IconButton {
            id: btnMine
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: if(checked) currentIndex = 2;
        }
        IconButton {
            id: btnSettings
            width: 40
            height: 40
            checkable: true
            exclusiveGroup: exclusiveGroup
            onCheckedChanged: if(checked) currentIndex = 3;
        }

    }

    Avatar {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: name.top
            bottomMargin: appTheme.narrowSpacing
        }
    }

    Text {
        id: name
        text: qsTr("Alita")
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.mainTextColor
        horizontalAlignment: Text.AlignHCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: appTheme.largeSpacing
        }
    }
}
