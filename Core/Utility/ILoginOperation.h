#ifndef ILOGINOPERATION_H
#define ILOGINOPERATION_H

#include "../User.h"

#include <QObject>
#include <QString>
#include <QSharedPointer>

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
     * @brief 发起网络请求，验证是否登录成功
     * @param mapping : 验证的数据键值对
     * @example
     * "account":"user1","password":"1234"
     * 或者："md5":"f2d2b9ae2d741a04634adfe18e2ea2c0"
     */
    virtual void verify(const QMap<QString, QString> &mapping) = 0;

    /**
     * @brief 注册相关账号，可以用该账号进行登录
     * @param mapping 注册时提供的数据键值对
     * 一般为：
     *      "name" : "username"
     *      "account" : "123465@qq.com"
     *      "password" : "pwd123"
     * @note 该方法不是必须继承实现的
     */
    virtual void signup(const QMap<QString, QString> &) {}

    /**
     * @brief 忘记密码方法，可以用该方法实现如何找回密码
     * @param verifyMethod  找回密码时提供的验证方式，通常为手机号或邮箱
     * @note 该方法不是必须继承实现的
     */
    virtual void forgetPassword(const QString &) {}

    static QSharedPointer<User> &getUser()
    {
        return mUser;
    }

protected:
    /**
     * @brief 解析登录时请求返回的json数据
     * @param jsonData 解析的json数据
     * @return User* 解析成功返回用户对像，解析失败返回nullptr
     */
    virtual QSharedPointer<User> parse(const QString &jsonData) = 0;

signals:
    // 信号：成功登录，
    void logedin();

    // 信号：成功注册账号
    void registered();

    // 信号：登录、注册失败时发送
    void failed(const QString &message = "");

protected:
    // 保存的用户信息
    static QSharedPointer<User> mUser;
};

#endif // ILOGINOPERATION_H

