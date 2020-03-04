import QtQuick 2.14
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.14

ListView {
    id: listView
    clip: true
    flickableDirection: Flickable.AutoFlickIfNeeded
    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightRangeMode: ItemView.StrictlyEnforceRange
    boundsBehavior: Flickable.OvershootBounds
    implicitWidth: emojiSize.width * columns
    contentHeight: currentItem.rows * emojiSize.height

    property var emojis: []
    property int columns: 1
    property size emojiSize: Qt.size(32, 32)
    property int contentTextSize: appTheme.smallTextSize

    property int leftInsert: 0
    property int rightInsert: 0
    property int topInsert: 0
    property int bottomInsert: 0

    signal selected(string emoji)

    Component.onCompleted: listView.model = emojis.length

    onContentHeightChanged: {
        if (contentHeight > height)
            flickableDirection = Flickable.AutoFlickIfNeeded
        else
            flickableDirection = Flickable.HorizontalFlick
    }

    delegate: GridLayout {
        id: grid
        columns: listView.columns
        rows: Number(emojis[grid.listIndex].emoji.length / columns + 0.5)
        columnSpacing: 0
        rowSpacing: 0

        readonly property int listIndex: index

        Repeater {
            model: emojis[grid.listIndex].emoji.length

            Rectangle {
                width: emojiSize.width
                height: emojiSize.height
                radius: appTheme.stdRadius

                Text {
                    id: text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: leftInsert
                    rightPadding: rightInsert
                    topPadding: topInsert
                    bottomPadding: bottomInsert
                    font.pixelSize: contentTextSize
                    color: appTheme.mainTextColor
                    text: emojis[grid.listIndex].emoji[index]
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: parent.color = appTheme.tintColor
                    onExited: parent.color = "transparent"
                    onPressed: parent.color = appTheme.subColor1
                    onClicked: {
                        parent.color = appTheme.tintColor
                        listView.selected(text.text)
                    }
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar {
        width: 4
        anchors.right: listView.right
        anchors.top: listView.top
        anchors.bottom: listView.bottom
    }

    Rectangle {
        width: 4
        radius: 2
        color: appTheme.subColor1
        anchors.right: listView.right
        height: listView.visibleArea.heightRatio * listView.height
        y: listView.visibleArea.yPosition * listView.height
    }
}
