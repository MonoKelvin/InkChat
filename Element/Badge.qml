import QtQuick 2.14
import QtQuick.Controls 2.0

Rectangle {
    property alias text: content.text
    property alias contentText: content

    implicitHeight: content.implicitHeight + 2
    implicitWidth: Math.max(content.implicitWidth, implicitHeight)
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
