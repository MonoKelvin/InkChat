#ifndef USER_H
#define USER_H

#include <IPerson.h>
#include <QQmlListProperty>

class MyFriend;

class User : public IPerson {
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(QString Account READ getAccount WRITE setAccount CONSTANT)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(QQmlListProperty<MyFriend> Friends READ getFriends)

public:
    explicit User(QObject* parent = nullptr);
    ~User() override;

    // TODO: next version
    struct SMyGroupData {
        unsigned int ID;
        unsigned int MemberCount;
        QString GroupName;
        QString Signature;
        QString Thumb;

        // QJsonObject toJsonObject(void);
    };

    // TODO: achieve it.
    struct SLocalAreaNetworkData {
        unsigned int MemberCount;
        QString Label;
    };

    void fromJson(const QJsonObject& json) override;
    QJsonObject toJson() override;

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }

    QQmlListProperty<MyFriend> getFriends(void);

    void addFriend(MyFriend* myFriend);

protected slots:
    bool cacheData() override;
    bool loadData() override;

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 我的好友
    QList<MyFriend*> mFriends;
};

#endif // USER_H
