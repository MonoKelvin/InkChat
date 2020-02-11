import QtQuick 2.0

Rectangle {
    id: header

    property alias title: titleText.text

    Rectangle {
        id: borderRadius
        color: parent.color
        visible: (parent.radius == 0)
        width: parent.width
        height: parent.radius
    }

    Text {
        id: titleText
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.mainTextColor
        //        height: header.height
        //        horizontalAlignment: Text.AlignHCenter
        //        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
    }
}
