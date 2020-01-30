import QtQuick 2.7
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import "qrc:/Navigation/"

ApplicationWindow {
    id: applicationWindow
    title: "InkChat"
    width: 800
    height: 600
    visible: true
    opacity: 0
    color: "#f5f6fa"

    Navigation {

    }

    NumberAnimation {
        id: opacityAnimation
        target: applicationWindow
        property: "opacity"
        easing.type: Easing.OutCubic
        duration: 300
        to: 1
    }
}
