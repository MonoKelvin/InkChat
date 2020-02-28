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
     * @brief 初始化登录页面。针对QML作为界面开发而使用
     * @param qmlEngine QML应用的解析引擎
     * @param url 登录页面的资源地址
     */
    static void InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url);

    static inline QPointer<QQmlApplicationEngine> GetQmlApplicationEngine()
    {
        return QmlEngine;
    }

    /**
     * @brief 自动登录请求
     * @return 当自动登录不可用（通常配置文件中未配置自动登录选项）时返回false，否则返回true
     * @note 任何自动登录的错误信息通过failed信号发出
     */
    Q_INVOKABLE void autoLoginRequest();

    Q_INVOKABLE void loginRequest(const QVariantMap& mapping = QVariantMap()) override;
    Q_INVOKABLE void signupRequest(const QVariantMap& mapping) override;
    Q_INVOKABLE void redirect(QQmlApplicationEngine* engine, const QUrl& url) override;

Q_SIGNALS:
    /** 信号：自动登录成功 */
    void autoLogin(const QString& account, const QString& password);

private:
    static QPointer<QQmlApplicationEngine> QmlEngine;
};

#endif // LOGINWITHQQMAIL_H
