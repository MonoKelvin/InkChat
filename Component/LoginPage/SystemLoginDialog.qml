import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as Utility

MouseArea {
    id: root
    anchors.fill: window.contentItem
    preventStealing: true
    visible: false
    z: 300

    Rectangle {
        id: offlineModelRect
        visible: parent.visible
        width: 320
        height: 280
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
        Behavior on height {
            NumberAnimation {
                easing.type: Easing.OutQuart
                duration: 200
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
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: appTheme.stdTextSize
                color: appTheme.mainTextColor
                Layout.fillWidth: true
                Layout.minimumHeight: 30
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
                    spacing: appTheme.stdSpacing

                    Text {
                        y: appTheme.stdSpacing
                        wrapMode: Text.Wrap
                        font.pixelSize: appTheme.smallTextSize
                        color: appTheme.subTextColor
                        text: qsTr("使用离线模式登录，就无法连接远程数据库、无法创建新的用户，但应用可以连接局域网络并且会保存你的聊天记录和相关配置数据。\n注：若忘记密码或无法读取离线数据，可以删除应用目录“./Data/0/”下的所有文件，同时所有数据也会被清空。")
                        width: parent.width
                    }

                    InputBox {
                        id: lockInputer
                        placeholderText: qsTr("解锁密码")
                        echoMode: InputBox.Password
                        width: parent.width
                        maximumLength: 16
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
                    spacing: appTheme.stdSpacing
                    x: loginArea.x + loginArea.width

                    Text {
                        y: appTheme.stdSpacing
                        wrapMode: Text.Wrap
                        font.pixelSize: appTheme.smallTextSize
                        color: appTheme.subTextColor
                        text: qsTr("使用离线模式创建新用户，原用户所有数据将会被清空，原因是应用在离线状态下暂不支持多用户的使用。")
                        width: parent.width
                    }

                    InputBox {
                        id: nickNameInputer
                        placeholderText: qsTr("*昵称（1-16位字符）")
                        width: parent.width
                        maximumLength: 16
                    }
                    InputBox {
                        id: pwdInputer
                        eyeVisible: true
                        placeholderText: qsTr("*密码（6-16位字符）")
                        echoMode: InputBox.Password
                        width: parent.width
                        maximumLength: 16
                    }
                    Row {
                        spacing: appTheme.narrowSpacing

                        Text {
                            text: qsTr("性别：")
                            font.pixelSize: appTheme.stdTextSize
                            verticalAlignment: Text.AlignVCenter
                            color: appTheme.mainTextColor
                            height: parent.implicitHeight
                        }

                        ButtonGroup {
                            id: genderGroup
                        }
                        StyleCheckBox {
                            text: qsTr("男")
                            ButtonGroup.group: genderGroup
                        }
                        StyleCheckBox {
                            text: qsTr("女")
                            ButtonGroup.group: genderGroup
                        }
                        StyleCheckBox {
                            text: qsTr("保密")
                            ButtonGroup.group: genderGroup
                            checked: true
                        }
                    }
                }
            }


            RowLayout {
                Layout.fillWidth: true
                Layout.maximumHeight: appTheme.stdWidgetHeight
                height: appTheme.stdWidgetHeight

                StyleButton {
                    radius: appTheme.stdRadius
                    text: qsTr("取消")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onClicked: root.visible = false
                }
                StyleButton {
                    radius: appTheme.stdRadius
                    text: qsTr("确定")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onClicked: {
                        if(newUserBtn.isNewUser) {
                            if(nickNameInputer.text.trim() === "")
                                Utility.createToast(qsTr("昵称不可为空"), window)
                            else if(pwdInputer.text.length < 6 || pwdInputer.text.length > 16)
                                Utility.createToast(qsTr("请输入6-16位的密码"), window)
                            else {
                                let gender = '-'

                                switch(genderGroup.checkedButton) {
                                case genderGroup.buttons[0]:
                                    gender = '1'
                                    break
                                case genderGroup.buttons[1]:
                                    gender = '0'
                                    break
                                default:
                                    break
                                }

                                loginWithOffline.signupRequest({
                                                                   "nickName": nickNameInputer.text,
                                                                   "password": pwdInputer.text,
                                                                   "gender": gender
                                                               });
                            }
                        } else {
                            if(lockInputer.text === "")
                                Utility.createToast(qsTr("请输入解锁密码"), window)
                            else
                                loginWithOffline.loginRequest({"password": lockInputer.text})
                        }
                    }
                }
                StyleButton {
                    id: newUserBtn
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: appTheme.stdRadius
                    text: qsTr("新用户")

                    property bool isNewUser: false

                    onClicked: {
                        isNewUser = !isNewUser

                        if(isNewUser) {
                            loginArea.x = -formArea.width
                            offlineModelRect.height += 40
                            text = qsTr("返回")
                        } else {
                            loginArea.x = 0
                            offlineModelRect.height -= 40
                            text = qsTr("新用户")
                        }
                    }
                }
            }
        }
    }
}
