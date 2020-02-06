import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import LoginWithQQMail 1.0
import "qrc:/Element/"
import "qrc:/js/js/Utility.js" as ToastJs

Window {
    id: window
    title: "InkChat"
    width: 810
    height: 518
    visible: true
    opacity: 0
    color: appTheme.backgroundColor

    minimumWidth: 720
    minimumHeight: 460

    // 当登录失败或者注册失败时被调用的槽函数，提供给C++
    function slotFailed(msg) {
        console.log(msg)
    }

    // 当登录成功时被调用的槽函数
    function slotVerified() {
        console.log('verified')
    }

    // 当注册成功时被调用的槽函数
    function slotRegistered() {
        console.log('registered')
    }

    Component.onCompleted: {
        opacityAnimation.start();
        loginOperation.failed.connect(slotFailed);
    }

    NumberAnimation {
        id: opacityAnimation
        target: window
        property: "opacity"
        duration: 300
        to: 1
    }

    AppTheme {
        id: appTheme
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
//            source: "http://inkchat.com/api/image.php?user=user&type=avatar&id=1"
            source: "https://hbimg.huabanimg.com/e6cf965a1906a001fdfc74d5a575408b396492d6c20a8-Ad4g9x_fw658"
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
        property bool isLogin: true

        id: form
        height: parent.height
        color: appTheme.backgroundColor
        anchors{
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
                validator: RegExpValidator { regExp: /[0-9a-zA-Z]{1,13}@qq\.com/ }
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
                text: '<a href="http://inkchat.com/api/forget_pwd"><span style="text-decoration: underline;color: '+appTheme.primaryColor1+';">'+qsTr('忘记密码？')+'</span></a>'
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
                    if (ibAccount.text.trim() === '' ||
                            ibPassword.text.trim() === '' ||
                            (!form.isLogin && ibNickName.text.trim() === ''))
                    {
                        ToastJs.createToast(qsTr("内容不允许为空"), window);
                        return;
                    }

                    if(!form.isLogin) {
                        if(ibPassword.text.length < 6) {
                            ToastJs.createToast(qsTr("密码至少位6个字符"), window);
                            return;
                        }

                        if(ibRecheckPwd.text !== ibPassword.text){
                            ToastJs.createToast(qsTr("两次输入的密码不一样"), window);
                            return;
                        }
                    }

                    if(form.isLogin) {
                        // 打包数据成json交付给C++处理
                        loginOperation.login({
                                                  account: ibAccount.text,
                                                  password: ibPassword.text
                                              });
                    } else {
                        loginOperation.signup({
                                                   nickName: ibNickName.text,
                                                   account: ibAccount.text,
                                                   password: ibPassword.text
                                               });
                    }
                }
            }

            RoundedButton {
                id: signupButton
                width: parent.width
                text: qsTr("注册")
                height: 40
                onClicked: form.isLogin = !form.isLogin;
            }
        }

        onIsLoginChanged: {
            if (isLogin) {
                loginButton.text = qsTr("登录");
                signupButton.text = qsTr("注册");

                ibNickName.visible = false;
                ibRecheckPwd.visible = false;
                forgetPwdText.visible = true;

                ibAccount.placeholderText = qsTr("账号");
                ibPassword.placeholderText = qsTr("密码");
            } else {
                loginButton.text = qsTr("注册");
                signupButton.text = qsTr("返回登录");

                ibNickName.visible = true;
                ibRecheckPwd.visible = true;
                forgetPwdText.visible = false;

                ibAccount.placeholderText = qsTr("账号（只支持QQ邮箱）");
                ibPassword.placeholderText = qsTr("密码（6-16位数字或字母）");
            }
        }
    }
}
