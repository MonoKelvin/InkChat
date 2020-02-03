import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import User 1.0
import "qrc:/Element/"

Item {

    Toast{id: toast;Component.onCompleted: toast.show("hello world");}

    Component.onCompleted: {
        groupListView.refresh([
                                  {
                                      groupName: "默认分组",
                                      groupMaxCount: 200,
                                      members: [
                                          {
                                              type: User.Friend,
                                              id: 1,
                                              avatar: "https://static.collectui.com/shots/3691930/sign-in-and-sign-up-views-dark-theme-large",
                                              onlineState: User.Online,
                                              name: "墨痕轻琐",
                                              signature: "阶前雨下客，窗后书中人",
                                          },
                                          {
                                              type: User.Stranger,
                                              id: 1250,
                                              avatar: "",
                                              onlineState: User.Online,
                                              name: "一个小组",
                                              signature: "组的介绍签名",
                                              friendCount: 36,
                                              onlineDays: 233
                                          },
                                          {
                                              type: User.Stranger,
                                              id: 51310,
                                              avatar: "",
                                              onlineState: User.Online,
                                              name: "一个小组",
                                              signature: "组的介绍签名",
                                              friendCount: -1,
                                              onlineDays: -1
                                          },
                                      ]
                                  },
                                  {
                                      groupName: "我的好友",
                                      groupMaxCount: 100,
                                      members: [
                                          {
                                              type: User.Friend,
                                              id: 1,
                                              avatar: "",
                                              onlineState: User.Online,
                                              name: "墨痕轻琐",
                                              signature: "阶前雨下客，窗后书中人",
                                          },
                                          {
                                              type: User.Friend,
                                              id: 451,
                                              avatar: "",
                                              onlineState: User.Offline,
                                              name: "墨痕轻琐",
                                              signature: "阶前雨下客，窗后书中人",
                                          },
                                          {
                                              type: User.Friend,
                                              id: 234,
                                              avatar: "",
                                              onlineState: User.Busy,
                                              name: "墨痕轻琐",
                                              signature: "阶前雨下客，窗后书中人。阶前雨下客，窗后书中人",
                                          },
                                          {
                                              type: User.Friend,
                                              id: 34241,
                                              avatar: "",
                                              onlineState: User.NoneState,
                                              name: "墨痕轻琐",
                                              signature: "阶前雨下客，窗后书中人。阶前雨下客，窗后书中人。阶前雨下客，窗后书中人",
                                          },
                                          {
                                              type: User.Stranger,
                                              id: 52340,
                                              avatar: "",
                                              onlineState: User.Online,
                                              name: "一个小组",
                                              signature: "组的介绍签名",
                                              friendCount: -1,
                                              onlineDays: 200
                                          },
                                      ]
                                  }
                              ]);
    }

    // 标题栏
    Rectangle {
        id: header
        width: parent.width
        height: 66
        z: 10
        color: appTheme.backgroundColor
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 20.0
            samples: 17
            color: appTheme.shadowColor
            verticalOffset: 1
        }

        //        BlurEffect {
        //            id: blurEffect
        //            target: titleBar
        //            background: chatListView
        //            blurRadius: 128
        //        }

        Text {
            id: titleNameText
            height: parent.height
            text: qsTr("好友列表")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            font.pixelSize: appTheme.titleTextSize
        }

        InputBox {
            id: searchBox
            width: header.width / 4
            placeholderText: qsTr("搜索...")
            icon: IconButton {
                clickedColor: appTheme.primaryColor1
                checked: true
            }
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: appTheme.stdSpacing
            }
        }
    }

    ListView {
        id: groupListView
        anchors.fill: parent
        anchors.topMargin: header.height + appTheme.wideSpacing
        anchors.leftMargin: appTheme.wideSpacing
        anchors.rightMargin: appTheme.wideSpacing
//        cacheBuffer: 9999
        spacing: appTheme.narrowSpacing

        model: ListModel {
            id: groupListModel
        }

        delegate: Loader {
            width: groupListView.width
            source: "FriendListView.qml"
        }

        function refresh( items ) {
            groupListModel.clear();

            for (var index = 0; index < items.length; index++) {
                var buf = items[index];
                groupListModel.append({
                                          _groupName: buf["groupName"],
                                          _groupMaxCount: buf["groupMaxCount"],
                                          _members: buf["members"],
                                      });
            }
        }
    }
}
