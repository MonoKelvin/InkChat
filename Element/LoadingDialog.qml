import QtQuick 2.14
import QtQuick.Controls 2.0
import "qrc:/js/js/Utility.js" as Utility

Rectangle {
    property bool modality: true
    property color dotColor: appTheme.primaryColor1

    id: background
    opacity: 0
    visible: false
    color: Utility.alpha(appTheme.mainTextColor, 0.3)
    z: Infinity

    // 取消信号，只有在非模态对话框状态点击屏幕后才会发出。
    signal cancel();

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    // 显示对话框，并设置文本内容
    function showDialog(parent, text) {
        contentText.text = text;
        background.anchors.fill = parent;
        background.visible = true;
        background.opacity = 1;
    }

    // 立即关闭
    function closeDialog() {
        background.opacity = 0;
        background.visible = false;
    }

    // 延迟关闭，可以设置一个文本。之后调用then函数
    function delayCloseDialog(text, then, delay = 1000) {
        contentText.text = text;

        closeTimer.triggered.connect(function (){
            background.visible = false;
            if(typeof then !== "undefined") {
                then();
            }
        });

        closeTimer.interval = delay;
        closeTimer.start();
        canvasTimer.stop();
    }

    onVisibleChanged: visible?canvasTimer.start():canvasTimer.stop();

    Timer {
        id: closeTimer
        interval: 1000
    }

    Rectangle {
        id: loading
        anchors.centerIn: parent
        color: appTheme.backgroundColor
        radius: appTheme.stdRadius
        width: 120
        height: 120

        readonly property real cRadius: 25
        readonly property real dotSize: 6
        property real curAngle: 0
        property real dotX: 25.0
        property real dotY: 3.0

        Canvas {
            id: canvas
            width: 2*(loading.cRadius + loading.dotSize)
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: appTheme.stdSpacing

            onPaint: {
                var ctx = canvas.getContext("2d");
                const r = loading.dotSize/2;

                // 清理画布
                ctx.clearRect(0, 0, canvas.width, canvas.height);

                // 绘制圆圈
                ctx.lineWidth = 1.5;
                ctx.strokeStyle = Utility.alpha(appTheme.mainTextColor, 0.1);
                ctx.beginPath();
                ctx.arc(loading.cRadius + r, loading.cRadius + r, loading.cRadius, 0, 360, true);
                ctx.stroke();

                // 绘制圆点
                ctx.lineWidth = 1.5;
                ctx.strokeStyle = appTheme.backgroundColor;
                ctx.fillStyle = background.dotColor;
                ctx.beginPath();
                ctx.ellipse(loading.dotX - r, loading.dotY - r, loading.dotSize, loading.dotSize);
                ctx.fill();
            }

            Timer {
                id: canvasTimer
                interval: 10
                repeat: true
                onTriggered: {
                    const r = loading.dotSize/2;
                    loading.dotX = r + loading.cRadius * (1 + Math.cos(loading.curAngle));
                    loading.dotY = r + loading.cRadius * (1 + Math.sin(loading.curAngle));

                    // 控制转速（到底部加速，到顶部减速）
                    loading.curAngle += 2 / (70 - loading.dotY);
                    if(loading.curAngle >= 2*Math.PI) {
                        loading.curAngle = 0.0;
                    }

                    canvas.requestPaint();
                }
            }

            Component.onCompleted: {
                canvasTimer.start();
            }
        }

        Text {
            id: contentText
            width: parent.width
            padding: appTheme.stdSpacing
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: appTheme.stdTextSize
            color: appTheme.mainTextColor
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: !modality
        onClicked: {
            if(!modality) {
                background.closeDialog();
                background.cancel();
            }
        }
    }
}
