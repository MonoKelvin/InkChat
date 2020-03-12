import QtQuick 2.14
import QtQuick.Controls 2.4

Button {
    id: root

    property alias radius: bg.radius
    property color normalColor: "transparent"
    property color hoveredColor: appTheme.tintColor
    property color pressedColor: appTheme.subColor1

    width: 24
    height: 24
    hoverEnabled: true
    focusPolicy: "NoFocus"
    font.pixelSize: appTheme.stdTextSize
    icon.color: appTheme.subTextColor
    padding: 0

    background: Rectangle {
        id: bg
        anchors.fill: parent
        color: {
            if (checked)
                return pressedColor
            return root.down ? pressedColor : (root.hovered ? hoveredColor : normalColor)
        }
    }
}
