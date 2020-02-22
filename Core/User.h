#ifndef USER_H
#define USER_H

#include <IChatObject.h>

#include <QPointer>
#include <QQmlListProperty>

class QJSEngine;
class MyFriend;
class LanObject;

class User : public IChatObject {
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(QString Account READ getAccount WRITE setAccount)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(QQmlListProperty<MyFriend> Friends READ getFriends)

protected:
    explicit User(QObject* parent = nullptr);

public:
    ~User() override;

    void fromJson(const QJsonObject& json, bool cache = true) override;
    QJsonObject toJson() override;

    bool hasCache();

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }

    QQmlListProperty<MyFriend> getFriends(void);

    /**
     * @brief 通过好友id获取我的好友
     * @param id 好友id
     * @return 如果id不存在将返回nullptr，否则返回好友对象
     */
    MyFriend* getFriendById(unsigned int id);

    /**
     * @brief 添加好友
     * @param myFriend 好友类
     */
    inline void addFriend(MyFriend* myFriend)
    {
        mFriends.append(myFriend);
    }

    /**
     * @brief 通过好友id获取我的局域网
     * @param id 局域网id
     * @return 如果id不存在将返回nullptr，否则返回局域网聊天对象
     */
    LanObject* getLanObjectById(unsigned int id);

    /**
     * @brief 添加一个局域网聊天对象
     * @param lan 局域网类
     */
    inline void addLanObject(LanObject* lan)
    {
        mLANs.append(lan);
    }

    static QObject* QmlSingletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return User::Instance();
    }

    static QPointer<User> Instance()
    {
        static QPointer<User> instance(new User);
        return instance;
    }

private:
    Q_DISABLE_COPY_MOVE(User)

    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 我的好友
    QList<MyFriend*> mFriends;

    // 局域网聊天对象
    QList<LanObject*> mLANs;

    // 群聊聊天对象
    // QList<GroupChat*> mGroups;
};

#endif // USER_H
