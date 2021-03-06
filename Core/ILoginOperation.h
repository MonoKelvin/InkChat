﻿#ifndef ILOGINOPERATION_H
#define ILOGINOPERATION_H

#include <User.h>

class QQmlApplicationEngine;

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
    explicit ILoginOperation(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~ILoginOperation() {}

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
     * @brief 忘记密码方法，可以用该方法实现如何找回密码
     * @param const QVariantMap& 找回密码时提供的验证键值对，通常为手机号或邮箱
     * @note 该方法不是必须继承实现的
     */
    virtual void forgetPassword(const QVariantMap&) {}

    /**
     * @brief 自动登录请求
     * @note 该方法不是必须继承实现的
     */
    virtual void autoLoginRequest() {}

public Q_SLOTS:
    /**
     * @brief 重定向方法。主要针对使用QWidget作为界面的开发
     * @param QWidget* 传入的QWidget控件
     * @note 在登录成功（或注册成功）后，虽然@see verified 信号是立刻发生的，但UI页面可能
     * 有自己的事务要处理，所以只有UI页面发出最后的关闭（或其他重定向信号）事件时，该方法才是最
     * 后需要调用的。
     */
    virtual void redirect(QWidget*) {}

    /**
     * @brief 重定向方法。主要针对使用QML作为界面的开发
     * @param const QUrl& 重定向的新页面地址
     * @param QQmlApplicationEngine* 传入的QML应用解析引擎
     * @note 在登录成功（或注册成功）后，虽然@see verified 信号是立刻发生的，但UI页面可能
     * 有自己的事务要处理，所以只有UI页面发出最后的关闭（或其他重定向信号）事件时，该方法才是最
     * 后需要调用的。
     */
    virtual void redirect(const QUrl&, QQmlApplicationEngine*) {}

Q_SIGNALS:
    /**
     * @brief 信号：成功登录
     * @note 一般在登录成功后跳转到主页面，还需要释放自身资源，即调用deleteLater
     */
    void verified();

    /**
     * @brief 信号：成功注册账号
     */
    void registered();

    /** 
     * @brief 信号：登录、注册失败时发送
     */
    void failed(const QString&);

    /**
     * @brief 信号：自动登录成功
     * @param const QVariantMap& 登录成功后传出的参数
     */
    void autoLogin(const QVariantMap&);

private:
    Q_DISABLE_COPY(ILoginOperation)
    Q_DISABLE_MOVE(ILoginOperation)
};

#endif // ILOGINOPERATION_H

