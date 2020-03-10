#ifndef LOGINDELEGATE_H
#define LOGINDELEGATE_H

#include <Http/ILoginOperation.h>
#include <QQmlApplicationEngine>

#define LOGINDELEGATE_INITIALIZA                                                \
    {                                                                           \
        qmlRegisterType<LoginDelegate>("LoginDelegate", 1, 0, "LoginDelegate"); \
    }

/**
 * @brief 登录代理类，用于保存注册所有登录方法
 */
class LoginDelegate : public QObject
{
    Q_OBJECT
public:
    explicit LoginDelegate(QObject* parent = nullptr);
    ~LoginDelegate();

    /**
     * @brief 基本登录方式id枚举
     */
    enum EBaseloginId {
        SystemLogin, // 系统登录
        QQMailLogin // QQ邮箱登录
    };
    Q_ENUM(EBaseloginId)

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
     * @brief 登录请求代理方法
     * @param mapping 登录提供的数据
     * @param loginId 登录方式id
     */
    Q_INVOKABLE inline void loginDelegate(const QVariantMap& mapping, int loginId = SystemLogin)
    {
        getLoginOperation(loginId)->loginRequest(mapping);
    }

    /**
     * @brief 自动登录代理方法
     */
    Q_INVOKABLE inline void autoLoginDelegate()
    {
        getLoginOperation(QQMailLogin)->autoLoginRequest();
    }

    /**
     * @brief 注册请求代理方法
     * @param mapping 登录提供的数据
     * @param loginId 登录方式id
     */
    Q_INVOKABLE inline void signupDelegate(const QVariantMap& mapping, int loginId = SystemLogin)
    {
        getLoginOperation(loginId)->signupRequest(mapping);
    }

    /**
     * @brief 添加一个登录选项
     * @param loginOp 登录选项类
     * @param loginId 登录id，用于区分不同的登录方式
     * TODO: 使用动态生成，参考IChatItem的构建方法
     */
    void addLoginOperation(ILoginOperation* loginOp, int loginId);

    /**
     * @brief 获取登录选项类
     * @param loginId 登录方式id
     * @return 如果存在该类则返回，否则返回false
     */
    inline ILoginOperation* getLoginOperation(int loginId)
    {
        for (auto i : mLoginOperations) {
            if (i.first == loginId) {
                return i.second;
            }
        }

        return nullptr;
    }

Q_SIGNALS:
    void verified();
    void registered();
    void failed(const QString&);
    void autoLogin(const QVariantMap&);

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
    Q_INVOKABLE void redirectDelegate(QQmlApplicationEngine* engine, const QUrl& url, int loginId);

private:
    static QPointer<QQmlApplicationEngine> QmlEngine;

    QList<QPair<int, ILoginOperation*>> mLoginOperations;
};

#endif // LOGINDELEGATE_H
