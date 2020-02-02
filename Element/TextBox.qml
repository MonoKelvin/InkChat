import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    property alias text: textBox.text
    property alias placeholder: textBox.placeholderText

    color: appTheme.widgetColor
    radius: appTheme.stdRadius

    Flickable {
        id: textBoxFlickable
        anchors.fill: parent
        anchors.margins: appTheme.tinySpacing
        contentWidth: width
        contentHeight: Math.max(textBox.implicitHeight, height)
        clip: true

        TextArea {
            id: textBox
            anchors.fill: parent
            selectByMouse: true
            wrapMode: TextEdit.WrapAnywhere
            color: appTheme.mainTextColor
            font.pixelSize: appTheme.stdTextSize
            selectedTextColor: appTheme.backgroundColor
            selectionColor: appTheme.primaryColor1
        }

//        Menu {
//            id: textBoxMenu
//            font.pixelSize: appTheme.stdTextSize

//            MenuItem {
//                text: qsTr("剪切")
//                onTriggered: {
//                    textBox.cut();
//                }
//            }
//        }

    }
}
