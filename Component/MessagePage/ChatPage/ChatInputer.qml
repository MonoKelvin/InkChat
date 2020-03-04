﻿import QtQuick 2.14
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as Utility

Item {
    id: root
    property int foldUpHeight
    property bool foldUp: false

    height: 180

    Component.onCompleted: foldUpHeight = height

    // 当发送消息时的信号
    signal sendChat(string content)

    onFoldUpChanged: {
        if (foldUp) {
            height = textBox.y + textBox.anchors.topMargin
            sendIcon.opacity = 0
            textBox.opacity = 0
            sendIcon.visible = 0
        } else {
            height = foldUpHeight
            sendIcon.opacity = 1
            sendIcon.visible = 1
            textBox.opacity = 1
        }

        anchors.bottom = parent.bottom
    }

    Behavior on height {
        NumberAnimation {
            easing.type: Easing.OutQuint
            duration: 450
        }
    }

    StyleButton {
        id: sendIcon
        width: 40
        height: 40
        radius: 20
        normalColor: appTheme.primaryColor1
        pressedColor: appTheme.primaryActiveColor1
        anchors {
            bottom: inputPanel.top
            bottomMargin: appTheme.narrowSpacing
            right: inputPanel.right
            rightMargin: appTheme.stdSpacing
        }
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }
        onClicked: {
            if (textBox.text !== '') {
                sendChat(textBox.text)
                textBox.text = ''
            }
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

        Item {
            anchors.fill: parent
            anchors.margins: appTheme.narrowSpacing

            Row {
                id: iconsToolBar
                spacing: appTheme.narrowSpacing
                width: parent.width
                height: textIcon.height

                StyleButton {
                    id: textIcon
                    checkable: true
                    checked: true
                }
                StyleButton {
                    id: emojiIcon
                    radius: appTheme.stdRadius

                    function onEmojiSelected(emoji) {
                        textBox.textBox.insert(textBox.textBox.cursorPosition,
                                               emoji)
                    }

                    onClicked: {
                        Utility.createEmojiPanel(x, root.y + 5, root.parent, onEmojiSelected)
                    }
                }
                StyleButton {
                    id: imageIcon
                    radius: appTheme.stdRadius
                }
                StyleButton {
                    id: videoIcon
                    radius: appTheme.stdRadius
                }
                StyleButton {
                    id: fileIcon
                    radius: appTheme.stdRadius
                }
                StyleButton {
                    id: flodUpIcon
                    radius: appTheme.stdRadius
                    onClicked: foldUp = !foldUp
                }
            }

            TextBox {
                id: textBox
                width: parent.width
                height: parent.height - y
                anchors.top: iconsToolBar.bottom
                anchors.topMargin: appTheme.narrowSpacing
                placeholder: qsTr("输入你想说的话...")
            }
        }
    }
}
