#ifndef USER_H
#define USER_H

#include <QQmlListProperty>

class MyFriend;

class User : public QObject
{
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(unsigned int ID READ getUID CONSTANT)
    Q_PROPERTY(QString NickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString Signature READ getSignature WRITE setSignature)
    Q_PROPERTY(char Gender READ getGender WRITE setGender)
    Q_PROPERTY(QString Avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString Account READ getAccount WRITE setAccount CONSTANT)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(EOnlineState OnlineState READ getOnlineState WRITE setOnlineState)
    Q_PROPERTY(QQmlListProperty<MyFriend> Friends READ getFriends)

public:
    explicit User() {}
    ~User();

    enum EOnlineState
    {
        Online,
        Offline,
        Busy,

        // 不显示左下角的状态
        NoneState,
    };
    Q_ENUM(EOnlineState)

    enum EUserType
    {
        Friend,
        Stranger,
        Group,
        BlackList,
    };
    Q_ENUM(EUserType)

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
        QString MyNickName;
    };

    inline const QString getNickName(void) const { return mNickName; }
    inline void setNickName(const QString& nickName)
    {
        mNickName = nickName;
        emit nickNameChanged(mNickName);
    }

    inline unsigned int getUID(void) const { return mUID; }

    inline QString getSignature(void) const { return mSignature; }
    inline void setSignature(const QString& value) { mSignature = value; }

    inline const QString getAccount(void) const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword(void) const { return mPassword; }

    inline const QString getAvatar(void) const { return mAvatar; }
    inline void setAvatar(const QString& value)
    {
        mAvatar = value;
        emit avatarChanged();
    }

    inline EOnlineState getOnlineState(void) const { return mOnlineState; }
    inline void setOnlineState(const EOnlineState& value) { mOnlineState = value; }

    inline char getGender(void) const { return mGender; }
    inline void setGender(char gender) { mGender = gender; }

    QQmlListProperty<MyFriend> getFriends(void);

protected slots:
    bool cacheUserData();

    //    bool cacheAvatar(const QUrl& path);

Q_SIGNALS:
    void nickNameChanged(const QString& nickName);
    void avatarChanged();

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    // 用户唯一标识
    unsigned int mUID;

    // 用户昵称
    QString mNickName;

    // 签名
    QString mSignature;

    // 性别。0男，1女，'\0'（空字符）保密
    char mGender;

    // 账号
    QString mAccount;

    // 密码
    QString mPassword;

    // 头像地址
    QString mAvatar;

    // 在线状态
    EOnlineState mOnlineState;

    // 我的好友
    QList<MyFriend*> mFriends;
};

#endif // USER_H
