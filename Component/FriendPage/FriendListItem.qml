import QtQuick 2.0
import User 1.0
import "qrc:/Element/"

Item {
    id: friendListItem
    clip: true

    Rectangle {
        id: friendCard
        anchors.fill: parent
        radius: appTheme.bigRadius
        color: appTheme.backgroundColor

        Avatar {
            id: avatar
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: appTheme.wideSpacing
            imageSource: _avatar
            onlineState: _onlineState
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: avatar.right
            anchors.leftMargin: appTheme.stdSpacing
            anchors.right: idTextContainer.left
            anchors.rightMargin: appTheme.wideSpacing
            spacing: appTheme.tinySpacing

            Text {
                font.bold: true
                font.pixelSize: appTheme.stdTextSize
                color: appTheme.mainTextColor
                text: _name
                width: parent.width
                elide: Text.ElideRight
            }
            Text {
                font.pixelSize: appTheme.smallTextSize
                color: appTheme.subTextColor
                text: _signature
                width: parent.width
                elide: Text.ElideRight
            }
        }

        Rectangle {
            id: idTextContainer
            color: appTheme.subColor2
            radius: appTheme.stdRadius
            height: appTheme.stdBadgeHeight
            width: idText.contentWidth + appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: appTheme.wideSpacing

            TextEdit {
                id: idText
                readOnly: true
                font.pixelSize: appTheme.smallTextSize
                anchors.centerIn: parent
                color: appTheme.primaryColor2
                text: "ID:" + _id

            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    idText.text = _id;
                    idText.selectAll();
                    idText.copy();
                    idText.deselect();
                    idText.text = "ID:" + _id;
                }
            }
        }
    }
}
