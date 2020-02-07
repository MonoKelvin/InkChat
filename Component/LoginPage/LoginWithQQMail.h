#ifndef LOGINWITHQQMAIL_H
#define LOGINWITHQQMAIL_H

#include <Http/ILoginOperation.h>
#include <QQmlApplicationEngine>

/**
 * @brief 使用QQ邮箱注册、登录和验证
 */
class LoginWithQQMail : public ILoginOperation
{
    Q_OBJECT
public:
    LoginWithQQMail(QObject *parent = nullptr);
    ~LoginWithQQMail() override;

    Q_INVOKABLE void loginRequest(const QVariantMap &mapping) override;
    Q_INVOKABLE void signupRequest(const QVariantMap &mapping) override;

    void parse(const QVariantMap &userJson) override;
    void redirect(QObject* ui) override;

    QQmlApplicationEngine Engine;

private:
    Q_DISABLE_COPY_MOVE(LoginWithQQMail)
};

#endif // LOGINWITHQQMAIL_H
