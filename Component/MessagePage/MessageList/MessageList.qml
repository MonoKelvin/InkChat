import QtQuick 2.14
import QtGraphicalEffects 1.0
import "qrc:/Element/"

Rectangle {
    id: messageList
    color: appTheme.backgroundColor

    signal itemClicked(var msgId)

    Text {
        id: title
        text: qsTr("消息列表")
        width: parent.width
        height: 45
        anchors.top: parent.top
        padding: appTheme.wideSpacing
        horizontalAlignment: Text.AlignHCenter
        color: appTheme.subTextColor
    }

    Item {
        id: toolBar
        width: parent.width
        anchors.top: title.bottom
        height: searchBox.height + 2 * appTheme.stdSpacing

        InputBox {
            id: searchBox
            placeholderText: qsTr("搜索...")
            icon: IconButton {
                clickedColor: appTheme.primaryColor1
                checked: true
            }
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: toolBtn.left
                leftMargin: appTheme.stdSpacing
                rightMargin: appTheme.tinySpacing
            }
        }

        IconButton {
            id: toolBtn
            anchors.right: parent.right
            anchors.rightMargin: appTheme.stdSpacing
            anchors.verticalCenter: parent.verticalCenter
            width: appTheme.stdWidgetHeight
            height: appTheme.stdWidgetHeight
            checked: true   //test
        }
    }

    ListView {
        id: messageListView
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        clip: true
        focus: true

        model: ListModel{
            id: messageListModel
        }

        delegate: Loader {
            id:loader
            width: messageListView.width
            source: "MessageItem.qml"
        }
    }

    IconButton {
        id: backToTop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: appTheme.wideSpacing
        width: 40
        height: 40
        radius: 20
        normalColor: appTheme.primaryColor1
        clickedColor: appTheme.primaryActiveColor1
        layer.enabled: true
        layer.effect: DropShadow {
//            anchors.fill: navigation
//            source: navigation
            radius: 12
            samples: 17
            color: Qt.lighter(appTheme.primaryColor1, 1.2)
            verticalOffset: 5
        }
    }

    function refresh(items) {
        messageListModel.clear();

        for (var index = 0; index < items.length; index++){
            var buf = items[index];
            messageListModel.append({
                                        _id: buf["id"],
                                        _avatar: buf["avatar"],
                                        _name: buf["name"],
                                        _message: buf["message"],
                                        _loginState: buf["loginState"],
                                        _unreadMessageNumber: buf["unreadMessageNumber"],
                                        _read: buf["read"],
                                        _messageTime: buf["messageTime"]
                                    });
        }
    }

}
