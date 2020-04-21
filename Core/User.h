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

    bool hasCache();

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
        assert(nullptr != chatObj);

        if (chatObj->parent() != this) {
            chatObj->setParent(this);
        }

        mMyChatObjects.append(chatObj);
    }

    /**
     * @brief 通过好友id获取我的好友
     * @param id 好友id
     * @return 如果id不存在将返回nullptr，否则返回好友对象
     */
    //MyFriend* getFriendById(unsigned int id);

    /**
     * @brief 通过id获取我的聊天对象，不推荐使用
     * @param id 聊天对象id
     * @return 如果id不存在将返回nullptr，否则返回聊天对象
     */
    //IChatObject* getChatObjectById(unsigned int id);

    IChatObject* getChatObjectByUuid(const QString& uuid);
    LanObject* getLanObjectByUuid(const QString& uuid);

    static QPointer<User> Instance()
    {
        static QPointer<User> instance = QPointer<User>(new User);
        return instance;
    }

private:
    void dynamicLoadCacheData(IChatObject* chatObj, const QString& fileName);

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
