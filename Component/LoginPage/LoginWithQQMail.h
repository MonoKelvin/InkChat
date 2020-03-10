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
    explicit LoginWithQQMail(QObject* parent = nullptr);
    ~LoginWithQQMail() override;

    void autoLoginRequest() override;
    void loginRequest(const QVariantMap& mapping = QVariantMap()) override;
    void signupRequest(const QVariantMap& mapping) override;
};

#endif // LOGINWITHQQMAIL_H
