import QtQuick 2.14
import QtQuick.Controls 2.0

Rectangle {
    property alias text: content.text
    property alias contentText: content

    height: content.implicitHeight + 2
    width: Math.max(content.implicitWidth, height)
    radius: appTheme.stdBadgeHeight / 2
    color: appTheme.primaryColor1

    Text {
        id: content
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: appTheme.smallTextSize
        color: appTheme.backgroundColor
    }
}
