#ifndef LOGINWITHQQMAIL_H
#define LOGINWITHQQMAIL_H

#include <Http/ILoginOperation.h>
#include <QQmlApplicationEngine>

#define LOGINOPERATION_INITIALIZA                                                     \
    {                                                                                 \
        qmlRegisterType<LoginWithQQMail>("LoginWithQQMail", 1, 0, "LoginWithQQMail"); \
    }

/**
 * @brief 使用QQ邮箱注册、登录和验证
 */
class LoginWithQQMail : public ILoginOperation
{
    Q_OBJECT
public:
    explicit LoginWithQQMail(QObject* parent = nullptr);
    ~LoginWithQQMail() override;

    /**
     * @brief 初始化登录页面。
     * @param qmlEngine QML应用的解析引擎
     * @param url 登录页面的资源地址
     */
    static void InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url);

    static inline QPointer<QQmlApplicationEngine> GetQmlApplicationEngine()
    {
        return QmlEngine;
    }

    Q_INVOKABLE void loginRequest(const QVariantMap& mapping = QVariantMap()) override;
    Q_INVOKABLE void signupRequest(const QVariantMap& mapping) override;
    Q_INVOKABLE void redirect(QQmlApplicationEngine* engine, const QUrl& url) override;

Q_SIGNALS:
    /** 信号：自动登录 */
    void autoLogin(const QString& account, const QString& password);

private:
    static QPointer<QQmlApplicationEngine> QmlEngine;
};

#endif // LOGINWITHQQMAIL_H
