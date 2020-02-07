#ifndef ILOGINOPERATION_H
#define ILOGINOPERATION_H

#include "../User.h"

#include <QString>

/**
 * @brief 登录方式抽象基类，以实现不同的登录方式
 * @note 继承该类，可以实现登录、注册、忘记密码等方法，可以发送登录成功、注册成功等信号，
 * 以及失败消息信号等。同时提供QML和QWidget接口，即能够实现与QML界面通信，也可以实现和
 * QWidget界面通信。
 */
class ILoginOperation : public QObject
{
    Q_OBJECT
public:
    ILoginOperation(QObject *parent = nullptr)
        : QObject(parent)
    {}

    virtual ~ILoginOperation()
    {
        if (!mUser.isNull())
        {
            mUser.clear();
            mUser = nullptr;
        }
    }

    /**
     * @brief 发起登录请求，验证是否登录成功
     * @param mapping : 验证的数据键值对
     */
    virtual void loginRequest(const QVariantMap &mapping) = 0;

    /**
     * @brief 注册相关账号，可以用该账号进行登录
     * @param const QVariantMap& 注册时提供的数据键值对
     * @note 该方法不是必须继承实现的
     */
    virtual void signupRequest(const QVariantMap &) {}

    /**
     * @brief 登录成功后重定向到新的页面方法
     * @param ui 要跳转到的新界面对象：
     * 1.如果要跳转到新的qml页面，则该参数需要传入 QQmlApplicationEngine* 对象，并重新设置
     *   Url指定新的qml页面。
     * 2.如果使用QWidget作为页面，则传入QWidget* 对象并重新设置页面属性。
     */
    virtual void redirect(QObject *ui) = 0;

    /**
     * @brief 忘记密码方法，可以用该方法实现如何找回密码
     * @param const QVariantMap& 找回密码时提供的验证键值对，通常为手机号或邮箱
     * @note 该方法不是必须继承实现的
     */
    virtual void forgetPassword(const QVariantMap &) {}

    static inline const QSharedPointer<User> &getUser() { return mUser; }

protected slots:
    /**
     * @brief 解析用户数据的方法
     * @param userJson 传入的用户json数据
     */
    virtual void parse(const QVariantMap &userJson) = 0;

signals:
    // 信号：成功登录，
    void verified();

    // 信号：成功注册账号
    void registered();

    // 信号：登录、注册失败时发送
    void failed(const QString &);

protected:
    // 保存的用户信息
    static QSharedPointer<User> mUser;

private:
    Q_DISABLE_COPY(ILoginOperation)
    Q_DISABLE_MOVE(ILoginOperation)
};

#endif // ILOGINOPERATION_H

