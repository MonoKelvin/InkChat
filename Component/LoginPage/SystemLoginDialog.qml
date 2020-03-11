import QtQuick 2.14
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import "qrc:/Element/"

MouseArea {
    id: root
    anchors.fill: window.contentItem
    preventStealing: true
    visible: false
    z: 300

    Rectangle {
        id: offlineModelRect
        visible: parent.visible
        width: 350
        height: 260
        anchors.centerIn: parent
        border.color: appTheme.tintColor
        radius: appTheme.stdRadius
        color: appTheme.backgroundColor
        layer.enabled: true
        layer.effect: DropShadow {
            id: dropShadow
            radius: 22
            samples: 20
            verticalOffset: 10
            color: appTheme.shadowColor
        }
        transform: Scale {
            id: scaleTrans
            origin.x: offlineModelRect.width / 2
            xScale: 0.0

            Behavior on xScale {
                NumberAnimation {
                    easing.type: Easing.OutQuart
                    duration: 200
                }
            }
        }

        onVisibleChanged: {
            if(visible) {
                scaleTrans.xScale = 1.0
            } else
                scaleTrans.xScale = 0.0
        }

        ColumnLayout {
            id: systemLoginPage
            anchors.fill: parent
            anchors.margins: appTheme.stdSpacing
            spacing: appTheme.narrowSpacing

            Text {
                text: qsTr("离线模式登录")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: appTheme.stdTextSize
                color: appTheme.mainTextColor
                Layout.fillWidth: true
            }

            // 表单区域
            Item {
                id: formArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                // 登录区域
                Column {
                    id: loginArea
                    width: parent.width
                    height: parent.height
                    topPadding: appTheme.stdSpacing
                    spacing: appTheme.stdSpacing

                    Text {
                        id: hintText
                        y: appTheme.stdSpacing
                        wrapMode: Text.Wrap
                        font.pixelSize: appTheme.smallTextSize
                        color: appTheme.subTextColor
                        text: qsTr("使用离线模式登录，就无法连接远程数据库、无法创建新的用户，但应用可以连接局域网络并且会保存你的聊天记录和相关配置数据。\n注：若忘记密码或无法读取离线数据，可以删除应用目录“./Data/0/”下的所有文件，同时所有数据也会被清空。")
                        width: parent.width
                    }

                    InputBox {
                        id: lockInputBox
                        placeholderText: qsTr("解锁密码")
                        echoMode: InputBox.Password
                        width: parent.width
                    }

                    Behavior on x {
                        NumberAnimation {
                            duration: 300
                            easing.type: Easing.OutQuart
                        }
                    }
                }

                // 注册区域
                Column {
                    id: registerArea
                    width: parent.width
                    height: parent.height
                    topPadding: appTheme.stdSpacing
                    spacing: appTheme.stdSpacing
                    x: loginArea.x + loginArea.width

                    InputBox {
                        id: nickNameInputer
                        placeholderText: qsTr("昵称")
                        echoMode: InputBox.Password
                        width: parent.width
                    }
                    InputBox {
                        id: passwordInputer
                        placeholderText: qsTr("密码")
                        echoMode: InputBox.Password
                        width: parent.width
                    }
                }
            }


            RowLayout {
                Layout.fillWidth: true
                height: appTheme.stdWidgetHeight

                StyleButton {
                    radius: appTheme.stdRadius
                    text: qsTr("取消")
                    Layout.fillWidth: true
                    height: parent.height
                    onClicked: root.visible = false
                }
                StyleButton {
                    radius: appTheme.stdRadius
                    text: qsTr("确定")
                    Layout.fillWidth: true
                    height: parent.height
                    onClicked: {
                        loginWithOffline.loginRequest({"password": ibPassword.text})
                    }
                }
                StyleButton {
                    Layout.fillWidth: true
                    height: parent.height
                    radius: appTheme.stdRadius
                    text: qsTr("新用户")

                    property bool isNewUser: false

                    onClicked: {
                        isNewUser = !isNewUser

                        if(isNewUser)
                            loginArea.x = -formArea.width
                        else
                            loginArea.x = 0
                    }
                }
            }
        }
    }
}
