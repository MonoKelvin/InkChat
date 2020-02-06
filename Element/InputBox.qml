import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

TextField {
    property color normalBackgroundColor: appTheme.tintColor
    property color focusBackgroundColor: appTheme.tintColor
    property var icon: null

    enum EIconAlignment {
        AlignLeft,
        AlignRight
    }

    property int iconAlignment: InputBox.AlignLeft

    id: textField
    height: appTheme.stdWidgetHeight
    clip: true
    color: appTheme.mainTextColor
    font.pixelSize: appTheme.stdTextSize
    leftPadding: appTheme.narrowSpacing
    rightPadding: appTheme.narrowSpacing
    placeholderTextColor: appTheme.subTextColor
    selectedTextColor: appTheme.backgroundColor
    selectionColor: appTheme.primaryColor1
    selectByMouse: true

    background: Rectangle {
        id: inputRect
        anchors.fill: parent
        radius: appTheme.stdRadius
        color: appTheme.widgetColor

        children: icon

        Component.onCompleted: {
            if(icon) {
                if(iconAlignment === InputBox.AlignRight) {
                    icon.anchors.right = right;
                    textField.rightPadding += appTheme.stdSpacing + icon.width;
                    icon.anchors.rightMargin = appTheme.tinySpacing;
                } else {
                    icon.anchors.left = left;
                    textField.leftPadding += appTheme.stdSpacing + icon.width;
                    icon.anchors.leftMargin = appTheme.tinySpacing;
                }

                icon.anchors.verticalCenter = verticalCenter;
                icon.height = height - appTheme.narrowSpacing;
                icon.width = icon.height;
            }
        }
    }
}
