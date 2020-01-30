import QtQuick 2.0
import QtQuick.Controls 2.0
import "qrc:/Element/"

Rectangle {

    // use "qrc:///" instead of ":/" as QML requires URL paths.
    property alias iconUrl: icon.source
    property size iconSize: Qt.size(16, 16)
    property bool isCircleBorder: false

    property color clickedColor: AppTheme.primaryColor1
    property color clickedIconUrl

    radius: isCircleBorder?width/2:6

    Image {
        id: icon
        anchors.centerIn: parent
        source: iconUrl
        sourceSize: iconSize
    }

    MouseArea {
        onClicked: {
            icon.source = clickedIconUrl;
            parent.color = clickedColor;
        }
    }
}
