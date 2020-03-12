import QtQuick 2.14
import QtQuick.Controls 2.5

CheckBox {
    id: control

    property color indicatorColor: appTheme.mainTextColor

    font.pixelSize: appTheme.stdTextSize

    indicator: Rectangle {
        width: 14
        height: 14
        radius: 7
        opacity: control.enabled ? 1.0 : 0.5
        border.color: indicatorColor
        border.width: 2
        x: control.leftPadding
        y: parent.height / 2 - height / 2

        Rectangle {
            width: 6
            height: 6
            radius: 3
            anchors.centerIn: parent
            color: indicatorColor
            scale: control.checked ? 1 : 0

            Behavior on scale {
                NumberAnimation {
                    easing.type: Easing.OutQuart
                    duration: 200
                }
            }
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.5
        color: appTheme.mainTextColor
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
