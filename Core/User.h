#ifndef USER_H
#define USER_H

#include <IChatObject.h>
#include <Singleton.h>

#include <QQmlListProperty>

class QJSEngine;
class MyFriend;

class User : public IChatObject, public Singleton<User> {
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(QString Account READ getAccount WRITE setAccount CONSTANT)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(QQmlListProperty<MyFriend> Friends READ getFriends)

public:
    explicit User(QObject* parent = nullptr);
    ~User() override;

    void fromJson(const QJsonObject& json, bool cache = true) override;
    QJsonObject toJson() override;

    bool hasCache();

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }

    QQmlListProperty<MyFriend> getFriends(void);

    MyFriend* getFriendById(unsigned int id);

    void addFriend(MyFriend* myFriend);

    static QObject* QmlSingletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return User::Instance();
    }

private:
    Q_DISABLE_COPY_MOVE(User)

    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 我的好友
    QList<MyFriend*> mFriends;
};

#endif // USER_H
