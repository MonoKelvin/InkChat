import QtQuick 2.14
import QtGraphicalEffects 1.0


/*
Rectangle {
    id: root

    property alias imageSource: image.source

    color: appTheme.backgroundColor

    Image {
        id: image
        fillMode: Image.PreserveAspectFit

        onStatusChanged: {
            if (status === Image.Ready) {
                eventArea.enabled = true
                x = (root.width - width) * 0.5
                y = (root.height - height) * 0.5
            }
        }

        ParallelAnimation {
            id: parallelAni
            running: false

            function startAni(x, y, w, h) {
                xAni.to = x
                yAni.to = y
                wAni.to = w
                hAni.to = h

                start()
            }

            NumberAnimation {
                id: xAni
                target: image
                easing.type: Easing.OutCubic
                duration: 300
                property: "x"
            }
            NumberAnimation {
                id: yAni
                target: image
                easing.type: Easing.OutCubic
                duration: 300
                property: "y"
            }
            NumberAnimation {
                id: wAni
                target: image
                easing.type: Easing.OutCubic
                duration: 300
                property: "width"
            }
            NumberAnimation {
                id: hAni
                target: image
                easing.type: Easing.OutCubic
                duration: 300
                property: "height"
            }
        }
    }

    MouseArea {
        id: eventArea
        anchors.fill: image
        enabled: false
        drag.target: image
        drag.axis: Drag.XAndYAxis
        hoverEnabled: true

        property double scaleFactor: 1.0

        onWheel: {
            if (parallelAni.running)
                return

            var d = scaleFactor

            if (wheel.angleDelta.y > 0) {
                if (scaleFactor > 10.0)
                    scaleFactor = 10.0
                else
                    scaleFactor += scaleFactor * 0.3
            } else {
                if (scaleFactor < 0.1)
                    scaleFactor = 0.1
                else
                    scaleFactor -= scaleFactor * 0.3
            }

            let w = image.sourceSize.width * scaleFactor
            let h = image.sourceSize.height * scaleFactor

            let x = image.x + wheel.x * (1 - scaleFactor / d)
            let y = image.y + wheel.y * (1 - scaleFactor / d)

            parallelAni.startAni(x, y, w, h)
        }
    }
}
*/
Rectangle {
    id: root

    property alias imageSource: image.source

    color: appTheme.backgroundColor

    Image {
        id: image
        fillMode: Image.PreserveAspectFit

        property real scale: 1.0
        transform: Scale {
            id: scaleTrans
            origin: Qt.vector3d(0, 0, 0)
            xScale: image.scale
            yScale: image.scale
        }
        Behavior on scale {
            NumberAnimation {
                easing.type: Easing.OutQuint
                duration: 400
            }
        }

        onStatusChanged: {
            if (status === Image.Ready) {
                eventArea.enabled = true
                x = (root.width - width) * 0.5
                y = (root.height - height) * 0.5
            }
        }
    }

    Rectangle {
        border.width: 2
        border.color: "red"
        width: image.width * image.scale
        height: image.height * image.scale
        x: image.x
        y: image.y
        z: 20
        color: "transparent"
    }

    MouseArea {
        id: eventArea
        anchors.fill: root
        enabled: false
        drag.target: image
        drag.axis: Drag.XAndYAxis
        hoverEnabled: true


        /*onMouseXChanged: {
            if (drag.active) {
                scaleTrans.origin.x = mouseX - image.x
            }
        }
        onMouseYChanged: {
            if (drag.active) {
                scaleTrans.origin.y = mouseY - image.y
            }
        }*/
        property point lastWheelPos

        function zoom(x, y, scaleFactor) {
            const dx = x - image.x, dy = y - image.y
            if (scaleTrans.origin.x !== dx || scaleTrans.origin.y !== dy) {
                image.x += (x - lastWheelPos.x) / scaleFactor
                image.y += (y - lastWheelPos.y) / scaleFactor
                scaleTrans.origin.x = dx
                scaleTrans.origin.y = dy
            }
        }

        onWheel: {
            let scaleFactor = image.scale

            zoom(wheel.x, wheel.y, scaleFactor)

            if (wheel.angleDelta.y > 0) {
                if (scaleFactor > 10.0)
                    scaleFactor = 10.0
                else
                    scaleFactor += scaleFactor * 0.3
            } else {
                if (scaleFactor < 0.1)
                    scaleFactor = 0.1
                else
                    scaleFactor -= scaleFactor * 0.3
            }

            image.scale = scaleFactor

            lastWheelPos.x = wheel.x
            lastWheelPos.y = wheel.y
        }
    }
}
