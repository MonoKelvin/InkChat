import QtQuick 2.14
import "qrc:/js/js/Utility.js" as Utility

PopupDialog {
    property alias text: contentText.text
    property color dotColor: appTheme.primaryColor1
    readonly property real cRadius: 25
    readonly property real dotSize: 6

    onVisibleChanged: visible ? canvasTimer.start() : canvasTimer.stop()

    contentItem: Canvas {
        property real curAngle: 0
        property real dotX: 25.0
        property real dotY: 3.0

        id: canvas
        width: 2 * cRadius + dotSize
        height: width
        anchors.horizontalCenter: parent.horizontalCenter

        onPaint: {
            var ctx = canvas.getContext("2d")
            const r = dotSize / 2

            // 清理画布
            ctx.clearRect(0, 0, canvas.width, canvas.height)

            // 绘制圆圈
            ctx.lineWidth = 1.6
            ctx.strokeStyle = Utility.alpha(appTheme.mainTextColor, 0.2)
            ctx.beginPath()
            ctx.arc(cRadius + r, cRadius + r, cRadius, 0, 360, true)
            ctx.stroke()

            // 绘制圆点
            ctx.lineWidth = 1.5
            ctx.strokeStyle = appTheme.backgroundColor
            ctx.fillStyle = dotColor
            ctx.beginPath()
            ctx.ellipse(dotX - r, dotY - r, dotSize, dotSize)
            ctx.fill()
        }

        Timer {
            id: canvasTimer
            interval: 10
            repeat: true
            onTriggered: {
                const r = dotSize / 2
                canvas.dotX = r + cRadius * (1 + Math.cos(canvas.curAngle))
                canvas.dotY = r + cRadius * (1 + Math.sin(canvas.curAngle))

                // 控制转速（到底部加速，到顶部减速）
                canvas.curAngle += 2 / (70 - canvas.dotY)
                if (canvas.curAngle >= 2 * Math.PI) {
                    canvas.curAngle = 0.0
                }

                canvas.requestPaint()
            }
        }

        Component.onCompleted: {
            canvasTimer.start()
        }
    }

    footerItem: Text {
        id: contentText
        padding: appTheme.stdSpacing
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: appTheme.stdTextSize
        color: appTheme.mainTextColor
        anchors.centerIn: parent
    }
}
