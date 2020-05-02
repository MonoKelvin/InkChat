#ifndef USER_H
#define USER_H

#include <IChatObject.h>

#include <QPointer>

class MyFriend;
class LanObject;

/**
 * @brief 用户类。整个系统中用户是唯一的
 */
class User : public IPerson {
    Q_OBJECT

protected:
    explicit User(QObject* parent = nullptr);

public:
    ~User() override;

    void fromJson(const QJsonObject& json, bool cache = true) noexcept(false) override;
    QJsonObject toJson() override;

    bool setAvatar(const QString& fileName) override;

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }
    inline void setPassword(const QString& pwd) { mPassword = pwd; }

    /**
     * @brief 添加聊天对象
     * @param chatObj 聊天对象
     */
    inline void addChatObject(IChatObject* chatObj)
    {
        Q_ASSERT(nullptr != chatObj);
        chatObj->setParent(this);
        mMyChatObjects.append(chatObj);
    }

    LanObject* getLanObjectByUuid(const QString& uuid);

    /**
     * @brief 通过uuid获取一个聊天对象
     * @param uuid 聊天对象的uuid
     * @param createIfNull 如果为true就当没有缓存时就创建一个陌生人对象，并指定该对象的
     * uuid，但不会缓存改聊天对象的数据。
     * @return 返回聊天对象指针
     */
    IChatObject* getChatObjectByUuid(const QString& uuid, bool createIfNull = false);

    static QPointer<User> Instance()
    {
        static QPointer<User> instance = QPointer<User>(new User);
        return instance;
    }

private:
    IChatObject* dynamicLoadCacheData(const QString& uuid, bool createIfNull = false);

private:
    Q_DISABLE_COPY_MOVE(User)

    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 我的聊天对象集合
    QList<IChatObject*> mMyChatObjects;
};

#endif // USER_H
