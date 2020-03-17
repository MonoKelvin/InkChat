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

    Q_PROPERTY(QString Account READ getAccount WRITE setAccount)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)

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
    MyFriend* getFriendById(unsigned int id);

    /**
     * @brief 通过id获取我的局域网
     * @param id 局域网id
     * @return 如果id不存在将返回nullptr，否则返回局域网聊天对象
     * @note 该方法是动态加载局域网对象，即如果内存中不存在则会到局域网缓存文件夹搜索对应的配
     * 置文件，如果依旧没有搜索到才会返回nullptr
     */
    LanObject* getLanObjectById(unsigned int id);

    /**
     * @brief 通过md5获取我的局域网
     * @param md5 局域网md5
     * @return 如果md5不存在将返回nullptr，否则返回局域网聊天对象
     * @note 该方法是动态加载局域网对象，即如果内存中不存在则会到局域网缓存文件夹搜索对应的配
     * 置文件，如果依旧没有搜索到才会返回nullptr
     */
    LanObject* getLanObjectByMd5(const QString& md5);

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

    // 我的聊天对象集合
    QList<IChatObject*> mMyChatObjects;
};

#endif // USER_H
