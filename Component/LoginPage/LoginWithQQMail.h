#ifndef LOGINWITHQQMAIL_H
#define LOGINWITHQQMAIL_H

#include <ILoginOperation.h>

#include <QVariantMap>

/**
 * @brief 使用QQ邮箱注册、登录和验证
 */
class LoginWithQQMail : public ILoginOperation
{
    Q_OBJECT
public:
    explicit LoginWithQQMail(QObject* parent = nullptr);
    ~LoginWithQQMail() override;

    Q_INVOKABLE void autoLoginRequest() override;
    Q_INVOKABLE void loginRequest(const QVariantMap& mapping = QVariantMap()) override;
    Q_INVOKABLE void signupRequest(const QVariantMap& mapping) override;
};

#endif // LOGINWITHQQMAIL_H
