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

        function updateViewport(r) {
            if(contentX > r.x) {
                contentX = r.x
            } else if (contentX + width <= r.x + r.width) {
                contentX = r.x + r.width - width;
            }

            if(contentY > r.y) {
                contentY = r.y
            } else if (contentY + height <= r.y + r.height) {
                contentY = r.y + r.height - height;
            }
        }

        TextArea {
            id: textBox
            anchors.fill: parent
            selectByMouse: true
            wrapMode: TextEdit.Wrap
            color: appTheme.mainTextColor
            font.pixelSize: appTheme.stdTextSize
            selectedTextColor: appTheme.backgroundColor
            selectionColor: appTheme.primaryColor1
            onCursorRectangleChanged: textBoxFlickable.updateViewport(cursorRectangle);
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
