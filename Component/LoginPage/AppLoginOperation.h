#ifndef APPLOGINOPERATION_H
#define APPLOGINOPERATION_H

#include <Http/ILoginOperation.h>

/**
 * @brief App系统登录选项，不提供远程登录
 */
class AppLoginOperation : public ILoginOperation
{
    Q_OBJECT

public:
    explicit AppLoginOperation(QObject* parent = nullptr);
    ~AppLoginOperation() override;

    /**
     * @brief 初始化登录页面。针对QML作为界面开发而使用
     * @param qmlEngine QML应用的解析引擎
     * @param url 登录页面的资源地址
     */
    static void InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url);

    /**
     * @brief 登录请求，只需要密码锁即可
     * @param mapping : 验证的数据键值对
     */
    Q_INVOKABLE void loginRequest(const QVariantMap& mapping) override;

    /**
     * @brief 注册用户
     * @param mapping : 注册用到的数据键值对
     */
    Q_INVOKABLE void signupRequest(const QVariantMap& mapping) override;

    /**
     * @brief 检查给定用户名的用户是否存在
     * @param userName 要检查用户的用户名
     * @return 如果存在则返回true，否则返回false
     */
    Q_INVOKABLE bool isUserExists(const QString& userName);

public Q_SLOTS:
    void redirect(const QUrl& url, QQmlApplicationEngine* engine = nullptr) override;

private:
    static QPointer<QQmlApplicationEngine> QmlEngine;
};

#endif // APPLOGINOPERATION_H
