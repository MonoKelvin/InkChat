#ifndef LOGINWITHQQMAIL_H
#define LOGINWITHQQMAIL_H

#include "Utility/ILoginOperation.h"

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
    void verify(const QMap<QString, QString> &mapping) override;
    void signup(const QMap<QString, QString> &mapping) override;
};

#endif // LOGINWITHQQMAIL_H
