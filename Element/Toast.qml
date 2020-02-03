import QtQuick 2.14
import QtQuick.Controls 2.0

/**
 * @brief Manager that creates Toasts dynamically
 */
Item {
    id: root

    readonly property int defaultTime: 3000
    readonly property int fadeTime: 500
    property int showTime: 3000

    enum EShowType {
        EmergeFromTop,
        EmergeFromBottom,
        FadeFromCenter
    }

    z: Infinity
    width: window.width
    height: window.height

    function show(text, duration) {
        theText.text = text;
        if(typeof duration !== "undefined"){
            if(duration >= 2*fadeTime) {
                showTime = duration;
            } else {
                showTime = 2*fadeTime;
            }
        } else {
            showTime = defaultTime;
        }
        anim.start();
    }

    Rectangle {
        opacity: 0
        color: appTheme.primaryColor1
        width: theText.implicitWidth + 2 * appTheme.stdSpacing
        height: theText.implicitHeight + 2 * appTheme.narrowSpacing
        radius: appTheme.smallRadius
        anchors.centerIn: parent

        Text{
            id: theText
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            color: appTheme.backgroundColor
            font.pixelSize: appTheme.stdTextSize
        }

        SequentialAnimation on opacity {
            id: anim

            running: false

            NumberAnimation {
                to: 0.9
                duration: fadeTime
            }
            PauseAnimation {
                duration: showTime
            }
            NumberAnimation {
                to: 0
                duration: fadeTime
            }

            onRunningChanged: if(!running) root.destroy();
        }
    }
}
