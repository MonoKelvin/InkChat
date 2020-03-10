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
     * @brief 登录请求，只需要密码锁即可
     * @param mapping : 验证的数据键值对
     */
    void loginRequest(const QVariantMap& mapping) override;

public Q_SLOTS:

    /**
     * @brief 重定向方法。针对使用QML作为界面的开发
     * @param engine 传入的QML应用解析引擎
     * @param url 重定向的新页面地址
     * @note 在登录成功（或注册成功）后，虽然@see verified 信号是立刻发生的，但UI页面可能
     * 有自己的事务要处理，所以只有UI页面发出最后的关闭（或其他重定向信号）事件时，该方法才是最
     * 后需要调用的。
     * @note 重定向后，在新的QML页面使用`UserModel`获取用户数据。
     */
    void redirect(QQmlApplicationEngine* engine, const QUrl& url) override;
};

#endif // APPLOGINOPERATION_H
