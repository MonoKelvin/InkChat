import QtQuick 2.14
import QtQuick.Controls 2.0

Rectangle {
    property bool modality: true

    id: background
    visible: false
    color: appTheme.dimColor
    z: Infinity

    // 弹出控件
    property alias popItem: popRect
    // 标题
    property var titleItem: null
    // 内容
    property var contentItem: null
    // 底部
    property var footerItem: null
    // 是否有关闭按钮
    property bool closeButton: true
    // 是否激活关闭按钮
    property alias closeButtonActive: closeBtn.enabled

    // 取消信号，只有在非模态对话框状态点击屏幕后或者手动关闭对话框才会发出。
    signal cancel

    // 显示对话框
    function showDialog(parent) {
        background.anchors.fill = parent
        background.visible = true
    }

    // 立即关闭
    function closeDialog() {
        cancel()
        background.visible = false
    }

    // 延迟关闭，可以设置一个文本。之后调用then函数
    function delayCloseDialog(then, delay) {
        closeTimer.triggered.connect(function () {
            background.visible = false
            if (then !== undefined) {
                then()
            }
        })

        closeTimer.interval = (typeof delay !== "number") ? 1000 : delay
        closeTimer.start()
    }

    MouseArea {
        anchors.fill: parent
        preventStealing: true
        onClicked: {
            if (!modality) {
                background.closeDialog()
                background.cancel()
            }
        }
    }

    Timer {
        id: closeTimer
    }

    Rectangle {
        id: popRect
        anchors.centerIn: parent
        color: appTheme.backgroundColor
        radius: appTheme.stdRadius
        width: 140

        Column {
            Item {
                id: title
                width: popRect.width
                children: titleItem
                implicitHeight: Math.max(closeBtn.height + 2*closeBtn.anchors.margins,
                                 titleItem ? titleItem.height : 20)
            }
            Item {
                id: content
                width: popRect.width
                children: contentItem
                implicitHeight: contentItem ? contentItem.height : 0
            }
            Item {
                id: footer
                width: popRect.width
                children: footerItem
                implicitHeight: Math.max(20, footerItem ? footerItem.height : 20)
            }
            Component.onCompleted: {
                popRect.height = title.implicitHeight + content.implicitHeight + footer.implicitHeight
            }
        }
    }

    StyleButton {
        id: closeBtn
        radius: 12
        normalColor: appTheme.backgroundColor
        anchors.right: popRect.right
        anchors.top: popRect.top
        anchors.margins: appTheme.tinySpacing
        text: "×"
        font.pixelSize: appTheme.titleTextSize
        textColor: appTheme.mainTextColor
        visible: closeButton

        onClicked: closeDialog()
    }
}
