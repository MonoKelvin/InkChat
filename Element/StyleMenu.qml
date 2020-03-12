import QtQuick 2.14
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.3

Menu {
    id: contextMenu
    z: Infinity
    visible: false

    property color borderColor: appTheme.tintColor
    property int borderWidth: 1
    property int bordeRadius: appTheme.stdRadius
    property color shadowColor: appTheme.shadowColor

    property color menuColor: appTheme.backgroundColor
    property color hoveredColor: appTheme.tintColor
    property color pressedColor: appTheme.tintColor

    property color normalTextColor: appTheme.mainTextColor
    property color hoveredTextColor: appTheme.mainTextColor
    property color pressedTextColor: Qt.lighter(appTheme.mainTextColor, 1.8)

    property int menuWidth: 200
    property int menuItemHeight: 40

    topPadding: 1
    bottomPadding: 1
    leftPadding: 1
    rightPadding: 1

    enter: Transition {
        NumberAnimation {
            target: menuBackground
            property: "width"
            easing.type: Easing.OutQuint
            duration: 200
            from: 0
            to: menuWidth + leftPadding + rightPadding
        }
    }
    exit: Transition {
        NumberAnimation {
            target: menuBackground
            property: "width"
            easing.type: Easing.OutQuint
            duration: 200
            from: menuWidth
            to: 0
        }
    }

    Component.onCompleted: {
        topPadding += borderWidth
        bottomPadding += borderWidth
        leftPadding += borderWidth
        rightPadding += borderWidth
    }

    background: Rectangle {
        id: menuBackground
        border.color: borderColor
        border.width: borderWidth
        radius: bordeRadius
        color: menuColor
        width: 0
        implicitWidth: width
        implicitHeight: menuItemHeight + parent.topPadding + parent.bottomPadding
        clip: true
        layer.enabled: true
        layer.effect: DropShadow {
            id: dropShadow
            radius: 22
            samples: 20
            verticalOffset: 10
            color: shadowColor
        }
    }


    delegate: MenuItem {
        id: menuItem
        implicitWidth: menuWidth
        implicitHeight: menuItemHeight

        // 展开箭头
        arrow: Canvas {
            x: parent.width - width
            implicitWidth: menuItemHeight
            implicitHeight: menuItemHeight
            visible: menuItem.subMenu
            onPaint: {
                var ctx = getContext("2d")

                ctx.lineWidth = 1.5
                ctx.lineCap = "round"
                ctx.strokeStyle = appTheme.subTextColor

                ctx.moveTo(18, 13)
                ctx.lineTo(width - 15, height / 2)
                ctx.lineTo(18, height - 13)
                ctx.stroke()
            }
        }

        // 指示器
        indicator: Item {
            implicitWidth: menuItemHeight
            implicitHeight: menuItemHeight

            /*Icon {
                id: iconImg
                anchors.centerIn: parent
                source: {
                    if (menuItem.action && menuItem.action.icon) {
                        const icon = menuItem.action.icon
                        iconImg.color = icon.color
                        if (icon.width > 1 && icon.height > 1) {
                            iconImg.width = icon.width
                            iconImg.width = icon.height
                        }
                        return icon.source
                    } else {
                        iconImg.visible = false
                    }

                    return ''
                }
            }*/

            Rectangle {
                width: 14
                height: 14
                radius: 7
                anchors.centerIn: parent
                visible: menuItem.checkable
                border.color: appTheme.mainTextColor
                border.width: 2

                Rectangle {
                    id: checkCircle
                    width: 6
                    height: 6
                    radius: 3
                    anchors.centerIn: parent
                    color: appTheme.mainTextColor
                    scale: menuItem.checked ? 1 : 0
                    Behavior on scale {
                        NumberAnimation {
                            easing.type: Easing.OutQuart
                            duration: 180
                        }
                    }
                }
            }
        }

        // 内容
        contentItem: Text {
            id: contentText
            opacity: enabled ? 1.0 : 0.5
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: appTheme.stdTextSize
            elide: Text.ElideRight
            leftPadding: menuItem.indicator.implicitWidth
            rightPadding: menuItem.arrow.implicitWidth
            text: menuItem.text
            color: normalTextColor
        }

        // 背景
        background: Rectangle {
            id: itemBg
            implicitWidth: menuWidth
            implicitHeight: menuItemHeight
            color: "transparent"
        }

        onPressedChanged: {
            if (pressed) {
                contentText.color = pressedTextColor
                itemBg.color = pressedColor
            }
        }

        onHighlightedChanged: {
            if (highlighted) {
                contentText.color = hoveredTextColor
                itemBg.color = hoveredColor
            } else {
                contentText.color = normalTextColor
                itemBg.color = "transparent"
            }
        }
    }
}
