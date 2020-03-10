import QtQuick 2.14
import QtQuick.Controls 2.4

Rectangle {
    property alias text: contentText.text
    property alias contentText: contentText

    height: contentText.implicitHeight
    width: Math.max(contentText.implicitWidth, height)
    radius: appTheme.stdBadgeHeight / 2
    color: appTheme.subColor2

    Text {
        id: contentText
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: appTheme.smallTextSize
        color: appTheme.primaryColor2
        padding: appTheme.narrowSpacing
    }
}
