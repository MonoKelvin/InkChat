#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class ILoginOperation;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() override;

    inline bool isExit() const { return mIsExit; }
    bool loggedIn();

private:
    void connections(void);

private:
    Ui::LoginDialog *ui;

    // 当前模式是否为注册用户
    bool mIsSignUp;

    // 是否退出登录或注册对话框
    bool mIsExit;

    // 登录接口类
    ILoginOperation *mLoginMethod;
};

#endif // LOGINDIALOG_H
