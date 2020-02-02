import QtQuick 2.14
import QtGraphicalEffects 1.0
import "qrc:/Element/"

Item {
    property int foldUpHeight
    property bool foldUp: false

    height: 200

    Component.onCompleted: foldUpHeight = height

    onFoldUpChanged: {
        if(foldUp) {
            height = iconsToolBar.height;
            sendIcon.opacity = 0;
        } else {
            height = foldUpHeight;
            sendIcon.opacity = 1;
        }

        textBox.focus = true;
        anchors.bottom = parent.bottom;
    }

    Behavior on height {
         NumberAnimation {
             easing.type: Easing.OutQuint
             duration: 450
         }
    }

    IconButton {
        id: sendIcon
        anchors{
            bottom: inputPanel.top
            bottomMargin: appTheme.narrowSpacing
            right: inputPanel.right
            rightMargin: appTheme.stdSpacing
        }
        width: 40
        height: 40
        radius: 20
        normalColor: appTheme.primaryColor1
        clickedColor: appTheme.primaryActiveColor1
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.InOutCubic
                duration: 450
            }
        }
    }

    Rectangle {
        id: inputPanel
        color: appTheme.backgroundColor
        anchors.fill: parent
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 20.0
            samples: 17
            color: appTheme.shadowColor
            verticalOffset: -1
        }

        Column {
            anchors.fill: parent
            anchors.margins: appTheme.narrowSpacing

            Row {
                id: iconsToolBar
                spacing: appTheme.narrowSpacing
                width: parent.width
                height: textIcon.height + 2 * appTheme.narrowSpacing

                IconButton {
                    id: textIcon
                    checkable: true
                    checked: true
                }
                IconButton {
                    id: imageIcon
                }
                IconButton {
                    id: videoIcon
                }
                IconButton {
                    id: fileIcon
                }
                IconButton {
                    normalColor: appTheme.primaryColor1
                    id: flodUpIcon
                    onClicked: foldUp = !foldUp
                }
            }

            TextBox {
                id: textBox
                width: parent.width
                height: parent.height - y
                placeholder: qsTr("输入你想说的话")
            }
        }
    }
}
