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
    /**
     * @brief 该构造函数主要针对使用QML作为界面开发时使用
     * @param url 登录页面的资源地址
     * @param contextName 添加到对应url地址的页面中的上下文对象，可直接在那个页面使用。
     * @warning 在声明对象前必须调用以下语句
     * LoginWithQQMail::SetQmlApplicationEngine();
     */
    LoginWithQQMail(const QUrl& url, const QString& contextName = QStringLiteral("loginOperation"));

    Q_INVOKABLE void loginRequest(const QVariantMap &mapping) override;
    Q_INVOKABLE void signupRequest(const QVariantMap& mapping) override;
    Q_INVOKABLE void redirect(QQmlApplicationEngine* engine, const QUrl& url) override;

    void parse(const QVariantMap& userJson) override;

    /**
     * @brief 设置QML应用的解析引擎。注意：必须先于该类的对象声明
     * @param qmlEngine QML应用的解析引擎
     * @example
     * 
     *  QQmlApplicationEngine engine;
     *  LoginWithQQMail::SetQmlApplicationEngine(&engine);
     *  //...
     *  LoginWithQQMail login("qrc:/LoginPage.qml", "loginOperation");
     *  //...
     * 
     */
    static inline void SetQmlApplicationEngine(QQmlApplicationEngine* qmlEngine)
    {
        QmlEngine = qmlEngine;
    }

    static inline QPointer<QQmlApplicationEngine> GetQmlApplicationEngine()
    {
        return QmlEngine;
    }

private:
    static QPointer<QQmlApplicationEngine> QmlEngine;

private:
    Q_DISABLE_COPY_MOVE(LoginWithQQMail)
};

#endif // LOGINWITHQQMAIL_H
