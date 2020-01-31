import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    property alias placeholder: textField.placeholderText
    property color normalBackgroundColor: appTheme.tintColor
    property color focusBackgroundColor: appTheme.tintColor
    property var icon: null

    id: textField
    height: appTheme.stdWidgetHeight
    clip: true
    textColor: appTheme.mainTextColor
    font.pixelSize: appTheme.stdTextSize

    style: TextFieldStyle {
        padding.left: 2*appTheme.tinySpacing + (icon?icon.width:0)
        background: Rectangle {
            id: inputRect
            anchors.fill: parent
            radius: appTheme.stdRadius
            color: appTheme.widgetColor

            children: icon

            Component.onCompleted: {
                if(icon) {
                    icon.anchors.verticalCenter = verticalCenter;
                    icon.anchors.left = left;
                    icon.anchors.leftMargin = appTheme.tinySpacing;

                    icon.height = height - 2*appTheme.tinySpacing;
                    icon.width = icon.height;
                }
            }
        }
    }
}
