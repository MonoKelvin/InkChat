import QtQuick 2.0
import QtQuick.Controls 2.4

Rectangle {
    property alias contentWidth: contentText.implicitWidth
    property alias text: contentText.text
    property alias textColor: contentText.color

    height: appTheme.stdBadgeHeight
    width: Math.max(contentText.contentWidth + appTheme.stdTextSize, height)
    radius: appTheme.stdBadgeHeight / 2
    color: appTheme.subColor3

    Text {
        id: contentText
        font.pixelSize: appTheme.smallTextSize
        anchors.centerIn: parent
        color: appTheme.primaryColor3
    }
}
