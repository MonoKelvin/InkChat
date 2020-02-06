#ifndef LOGINWITHQQMAIL_H
#define LOGINWITHQQMAIL_H

#include <Http/ILoginOperation.h>

/**
 * @brief 使用QQ邮箱注册、登录和验证
 */
class LoginWithQQMail : public ILoginOperation
{
    Q_OBJECT
public:
    LoginWithQQMail(QObject *parent = nullptr);

    // ILoginOperation interface
public:
    QSharedPointer<User> parse(const QString &jsonData) override;
    Q_INVOKABLE void login(const QVariantMap &mapping) override;
    Q_INVOKABLE void signup(const QVariantMap &mapping) override;

private:
    Q_DISABLE_COPY_MOVE(LoginWithQQMail)
};

#endif // LOGINWITHQQMAIL_H
