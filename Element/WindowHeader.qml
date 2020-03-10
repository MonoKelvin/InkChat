import QtQuick 2.0

MouseArea {
    id: header
    x: 0
    y: 0
    width: parent.width
    height: 36

    property point clickPos: "0,0"

    onPressed: clickPos  = Qt.point(mouse.x, mouse.y)

    onPositionChanged: {
        window.x += mouse.x - clickPos.x
        window.y += mouse.y - clickPos.y
    }

    // 关闭按钮
    StyleButton {
        id: closeBtn
        width: 26
        height: 26
        radius: 13
        normalColor: appTheme.backgroundColor
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: appTheme.tinySpacing
        text: "×"
        font.pixelSize: appTheme.titleTextSize
        textColor: appTheme.mainTextColor

        onClicked: window.close()
    }
}
