import QtQuick 2.7
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.0
import LoginWithQQMail 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as ToastJs

ApplicationWindow {
    id: window
    title: "InkChat"
    width: 810
    height: 518
    visible: true
    opacity: 0
    color: appTheme.backgroundColor
    flags: Qt.Window | Qt.FramelessWindowHint
    minimumWidth: 720
    minimumHeight: 460

    enum EAction {
        // 登录
        Login,
        // 注册
        Signup,
        // 退出或关闭登录界面
        Exit
    }

    // 是否是进行登录（否则为注册）
    property int action: LoginPage.Login

    // 是否正在进行请求，包括登录、注册请求
    property bool isRequesting: false

    Component.onCompleted: {
        opacityAnimation.start()
        autoLoginTimer.start()
        loadingDialog.text = qsTr("自动登录中...")
        loadingDialog.showDialog(window.contentItem)
    }

    onClosing: loginOperation.destroy()

    onIsRequestingChanged: {
        if (isRequesting) {
            loginButton.enabled = false
            signupButton.enabled = false
        } else {
            loginButton.enabled = true
            signupButton.enabled = true
        }
    }

    onActionChanged: {
        switch (window.action) {
        case LoginPage.Login:
            loginButton.text = qsTr("登录")
            signupButton.text = qsTr("注册")

            ibNickName.visible = false
            ibRecheckPwd.visible = false
            forgetPwdText.visible = true

            ibAccount.placeholderText = qsTr("账号")
            ibPassword.placeholderText = qsTr("密码")
            break
        case LoginPage.Signup:
            loginButton.text = qsTr("注册")
            signupButton.text = qsTr("返回登录")

            ibNickName.visible = true
            ibRecheckPwd.visible = true
            forgetPwdText.visible = false

            ibAccount.placeholderText = qsTr("账号（只支持QQ邮箱）")
            ibPassword.placeholderText = qsTr("密码（6-16位数字或字母）")
            break
        default:
            window.close()
            break
        }
    }

    AppTheme {
        id: appTheme
    }

    Timer {
        id: autoLoginTimer
        // 等待4s，看是否用户取消自动登录
        interval: 4000
        running: false
        onTriggered: {
            loginOperation.autoLoginRequest()
        }
    }

    LoginWithQQMail {
        id: loginOperation
        onFailed: function (msg) {
            ToastJs.createToast(msg, window)
            loadingDialog.closeDialog()
            isRequesting = false
        }
        onVerified: {
            loadingDialog.text = qsTr("登录成功")
            loadingDialog.delayCloseDialog(function () {
                // 动作为：退出登录页面
                action = LoginPage.Exit
                // 重定向到主页面
                redirect(null, "qrc:/main.qml")
            })
        }
        onRegistered: {
            loadingDialog.text = qsTr("注册成功")
            loadingDialog.delayCloseDialog()

            ibNickName.clear()
            ibAccount.clear()
            ibPassword.clear()
            ibRecheckPwd.clear()

            isRequesting = false
        }
        onAutoLogin: {
            ibAccount.text = account
            ibPassword.text = password
        }
    }

    LoadingDialog {
        id: loadingDialog
    }

    NumberAnimation {
        id: opacityAnimation
        target: window
        property: "opacity"
        duration: 300
        to: 1
    }

    // app信息展示区域
    Rectangle {
        id: infoPannel
        color: appTheme.tintColor
        height: parent.height
        width: parent.width * 0.618

        // todo: 替换成logo
        Image {
            id: appLogo
            source: "http://inkchat.com/api/image.php?user=user&size=80&id=1"
            width: parent.width
            fillMode: Image.PreserveAspectFit
            height: 140
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            anchors.top: parent.top
            anchors.topMargin: 100
        }

        Text {
            id: appNameText
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            font.pixelSize: 42
            text: qsTr("InkChat")
            color: appTheme.mainTextColor
            width: parent.width
            anchors.top: appLogo.bottom
            anchors.topMargin: appTheme.wideSpacing
        }

        Text {
            id: developerText
            textFormat: TextEdit.RichText
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            font.pixelSize: appTheme.smallTextSize
            color: appTheme.subTextColor
            text: '<span>Developed by MonoKelvin</span><span><br/>github: </span><a href="https://github.com/MonoKelvin"><span style="text-decoration: underline;color:#4D7CFE;">https://github.com/MonoKelvin</span></a>'
            width: parent.width
            anchors.bottom: parent.bottom
            bottomPadding: appTheme.wideSpacing
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    // 表单区域
    Rectangle {
        id: form
        height: parent.height
        color: appTheme.backgroundColor
        anchors {
            left: infoPannel.right
            right: parent.right
            leftMargin: appTheme.extremeSpacing
            rightMargin: appTheme.extremeSpacing
        }

        Text {
            id: loginTitleText
            text: qsTr("登录")
            width: parent.width
            height: contentHeight
            anchors.top: parent.top
            anchors.topMargin: appTheme.extremeSpacing
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: appTheme.titleTextSize
            color: appTheme.mainTextColor
        }

        Column {
            width: parent.width
            spacing: appTheme.narrowSpacing
            anchors {
                top: loginTitleText.bottom
                topMargin: appTheme.extremeSpacing
            }

            InputBox {
                id: ibNickName
                height: 40
                width: parent.width
                placeholderText: qsTr("昵称（1-16个字符）")
                maximumLength: 16
                visible: false
            }
            InputBox {
                id: ibAccount
                height: 40
                width: parent.width
                placeholderText: qsTr("账号")
                maximumLength: 36
                validator: RegExpValidator {
                    regExp: /[0-9a-zA-Z]{1,13}@qq\.com/
                }
            }
            InputBox {
                id: ibPassword
                height: 40
                width: parent.width
                placeholderText: qsTr("密码")
                echoMode: TextInput.Password
                maximumLength: 16
            }
            InputBox {
                id: ibRecheckPwd
                height: 40
                width: parent.width
                placeholderText: qsTr("确认密码")
                echoMode: TextInput.Password
                maximumLength: 16
                visible: false
            }

            Text {
                id: forgetPwdText
                width: parent.width
                text: '<a href="http://inkchat.com/api/forget_pwd"><span style="text-decoration: underline;color: '
                      + appTheme.primaryColor1 + ';">' + qsTr(
                          '忘记密码？') + '</span></a>'
                textFormat: Text.RichText
                horizontalAlignment: Text.AlignRight
                linkColor: appTheme.primaryColor1
                font.pixelSize: appTheme.stdTextSize
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }

        Column {
            width: parent.width
            spacing: appTheme.narrowSpacing
            anchors.bottom: parent.bottom
            anchors.bottomMargin: appTheme.wideSpacing

            RoundedButton {
                id: loginButton
                width: parent.width
                text: qsTr("登录")
                height: 40

                // 数据和合法验证
                onClicked: {
                    if (ibAccount.text.trim() === '' || ibPassword.text.trim(
                                ) === '' || (window.action === LoginPage.Signup
                                             && ibNickName.text.trim(
                                                 ) === '')) {
                        ToastJs.createToast(qsTr("内容不允许为空"), window)
                        return
                    }

                    if (ibPassword.text.length < 6) {
                        ToastJs.createToast(qsTr("密码至少位6个字符"), window)
                        return
                    }

                    if (window.action === LoginPage.Signup
                            && ibRecheckPwd.text !== ibPassword.text) {
                        ToastJs.createToast(qsTr("两次输入的密码不一样"), window)
                        return
                    }

                    isRequesting = true

                    if (window.action === LoginPage.Login) {
                        loginOperation.loginRequest({
                                                        "account": ibAccount.text,
                                                        "password": ibPassword.text
                                                    })
                        loadingDialog.showDialog(window.contentItem,
                                                 qsTr("正在登录..."))
                    } else {
                        loginOperation.signupRequest({
                                                         "nickName": ibNickName.text,
                                                         "account": ibAccount.text,
                                                         "password": ibPassword.text
                                                     })
                        loadingDialog.showDialog(window.contentItem,
                                                 qsTr("正在注册..."))
                    }
                }
            }

            RoundedButton {
                id: signupButton
                width: parent.width
                text: qsTr("注册")
                height: 40
                onClicked: {
                    if (window.action === LoginPage.Login)
                        window.action = LoginPage.Signup
                    else
                        window.action = LoginPage.Login
                }
            }
        }
    }
}
