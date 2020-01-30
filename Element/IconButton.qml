import QtQuick 2.1
import QtQuick.Controls 1.4
import "qrc:/Element/"

Rectangle {
    id: iconButton

    // use "qrc:///" instead of ":/" as QML requires URL paths.
    property url normalIcon: icon.source
    property url clickedIcon: normalIcon
    property size iconSize: Qt.size(16, 16)

    property bool circleBorder: false

    property bool checkable: false
    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null

    property color normalColor: "transparent"
    property color clickedColor: appTheme.tintColor

    width: 24
    height: 24
    radius: circleBorder?width/2:6
    color: checked?clickedColor:normalColor

    signal clicked;

    onExclusiveGroupChanged: {
        if(exclusiveGroup) {
            exclusiveGroup.bindCheckable(iconButton);
        }
    }

    //如果少了这个信号处理，则无法出现互斥效果
    onCheckedChanged: {
        if(checked) {
            color = clickedColor;
            icon.source = clickedIcon;
        } else {
            color = normalColor;
            icon.source = normalIcon;
        }
    }

    Image {
        id: icon
        anchors.centerIn: parent
        source: normalIcon
        sourceSize: iconSize
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if(exclusiveGroup && checked) {
                return;
            }
            checked = !checked;
            parent.clicked();
        }

        onReleased: {
            if(!checkable) {
                checked = !checked;
            }
        }
    }
}
