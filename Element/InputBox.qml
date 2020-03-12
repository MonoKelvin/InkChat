import QtQuick 2.0
import QtQuick.Controls 2.0
import QtLocation 5.14

TextField {
    property color normalBackgroundColor: appTheme.tintColor
    property color focusBackgroundColor: appTheme.tintColor
    property bool eyeVisible: false
    property url iconSource

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
        width: textField.width
        height: textField.height
        radius: appTheme.stdRadius
        color: appTheme.widgetColor

        Component.onCompleted: {
            if (iconSource != "") {
                var newObject = Qt.createQmlObject('import QtQuick 2.0; Image{ }', inputRect, "dynamicCreateIcon");
                if (newObject !== null) {
                    newObject.x = appTheme.narrowSpacing
                    newObject.anchors.verticalCenter = verticalCenter
                    newObject.height = 20
                    newObject.width = 20
                    newObject.source = iconSource
                    textField.leftPadding += 20 + appTheme.narrowSpacing
                }
            }
        }

        // 添加显隐密码的控件
        StyleButton {
            id: eye
            visible: eyeVisible
            checkable: true
            hoverEnabled: false
            pressedColor: "transparent"
            icon.name: checked ? "eye_open" : "eye_off"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: appTheme.narrowSpacing

            onCheckedChanged: {
                if(checked) echoMode = TextField.Normal
                else echoMode = TextField.Password
            }
        }
    }

    onEyeVisibleChanged: {
        if(eyeVisible && echoMode === TextField.Password) {
            eye.visible = true
            textField.rightPadding = appTheme.narrowSpacing + eye.implicitWidth
        } else {
            eye.visible = false
            textField.rightPadding = appTheme.narrowSpacing
        }
    }
}
