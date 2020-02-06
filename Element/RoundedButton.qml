import QtQuick 2.0

Rectangle {
    property color normalColor: appTheme.primaryColor1
    property color hoverColor: appTheme.primaryActiveColor1
    property color pressedColor: appTheme.primaryActiveColor1

    property color normalTextColor: appTheme.backgroundColor
    property color hoverTextColor: appTheme.backgroundColor
    property color pressedTextColor: appTheme.backgroundColor

    property alias hoverEnabled: mouseArea.hoverEnabled
    property alias text: contentText.text

    signal clicked();
    signal hover();
    signal exited();
    signal released();

    width: contentText.implicitWidth
    height: appTheme.stdWidgetHeight
    radius: appTheme.stdRadius
    color: normalColor
    transformOrigin: Item.Center

    Behavior on color {
        ColorAnimation {
            duration: 300
        }
    }

    Text {
        id: contentText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: appTheme.stdTextSize
        color: normalTextColor

        Behavior on color {
            ColorAnimation {
                duration: 300
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            parent.color = hoverColor;
            contentText.color = hoverTextColor;
            parent.hover();
        }
        onExited: {
            parent.color = normalColor;
            contentText.color = normalTextColor;
            parent.exited();
        }
        onClicked: {
            parent.color = pressedColor;
            contentText.color = pressedTextColor;
            parent.clicked();
        }
        onPressed: {
            parent.scale = 0.99;
        }
        onReleased: {
            parent.scale = 1;
            parent.released();
        }
    }
}
