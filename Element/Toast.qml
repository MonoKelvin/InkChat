import QtQuick 2.14
import QtQuick.Controls 2.0

Item {
    id: root

    property int showTime: 2500
    property int showType: Toast.EmergeFromBottom
    readonly property int fadeTime: 300

    // TODO: 目前只支持从底部冒出(EmergeFromBottom)
    enum EShowType {
        EmergeFromTop,
        EmergeFromBottom,
        FadeFromCenter
    }

    z: Infinity
    width: window.width
    height: window.height
    clip: true

    function show(text, duration, type) {
        theText.text = text
        if (duration !== undefined && duration > 2 * fadeTime)
            showTime = duration

        if (type !== undefined)
            showType = type
        anim.start()
    }

    Rectangle {
        id: toastItem
        opacity: 0
        color: appTheme.primaryColor1
        width: theText.implicitWidth + appTheme.wideSpacing
        height: theText.implicitHeight + appTheme.wideSpacing
        radius: appTheme.smallRadius

        Component.onCompleted: {
            toastItem.anchors.horizontalCenter = root.horizontalCenter
            toastItem.y = root.height

            // switch(showType) {
            // case Toast.EmergeFromTop:
            //     toastItem.y = -toastItem.height;
            //     break;
            // case Toast.EmergeFromBottom:
            //     toastItem.y = root.height;
            //     break;
            // default:
            //     toastItem.anchors.centerIn = root;
            //     break;
            // }
        }

        Text {
            id: theText
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            color: appTheme.backgroundColor
            font.pixelSize: appTheme.stdTextSize
        }

        SequentialAnimation {
            id: anim
            running: false

            ParallelAnimation {
                PropertyAnimation {
                    target: toastItem
                    property: "y"
                    duration: fadeTime
                    easing.type: Easing.OutCubic
                    to: root.height - toastItem.height - appTheme.wideSpacing
                }
                PropertyAnimation {
                    target: toastItem
                    property: "opacity"
                    duration: fadeTime
                    easing.type: Easing.OutCubic
                    to: 0.95
                }
            }
            PauseAnimation {
                duration: showTime
            }
            ParallelAnimation {
                PropertyAnimation {
                    target: toastItem
                    property: "y"
                    duration: fadeTime
                    easing.type: Easing.OutCubic
                    to: root.height
                }
                PropertyAnimation {
                    target: toastItem
                    property: "opacity"
                    duration: fadeTime
                    easing.type: Easing.OutCubic
                    to: 0
                }
            }

            onRunningChanged: {
                if (!running)
                    root.destroy()
            }
        }
    }
}
