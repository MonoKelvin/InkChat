#ifndef USER_H
#define USER_H

#include <QPixmap>
#include <thread>
#include <mutex>

class User : public QObject
{
    Q_OBJECT

    friend class LoginWithQQMail;

    Q_PROPERTY(unsigned int ID READ getUID CONSTANT)
    Q_PROPERTY(QString MD5 READ getMD5 CONSTANT)
    Q_PROPERTY(QString NickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString Signature READ getSignature WRITE setSignature)
    Q_PROPERTY(char Gender READ getGender WRITE setGender)
    Q_PROPERTY(QString Avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString Account READ getAccount WRITE setAccount CONSTANT)
    Q_PROPERTY(QString Password READ getPassword CONSTANT)
    Q_PROPERTY(EOnlineState OnlineState READ getOnlineState WRITE setOnlineState)

public:
    explicit User() {}

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

    const QString getNickName(void) const { return mNickName; }
    void setNickName(const QString& nickName)
    {
        mNickName = nickName;
        emit nickNameChanged();
    }

    inline unsigned int getUID() const { return mUID; }

    inline QString getSignature() const { return mSignature; }
    inline void setSignature(const QString& value) { mSignature = value; }

    inline const QString getMD5() const { return mMD5; }

    inline const QString getAccount() const { return mAccount; }
    inline void setAccount(const QString& value) { mAccount = value; }

    inline const QString getPassword() const { return mPassword; }

    inline const QString getAvatar() const { return mAvatar; }
    inline void setAvatar(const QString& value)
    {
        mAvatar = value;
        emit avatarChanged();
    }

    inline EOnlineState getOnlineState() const { return mOnlineState; }
    inline void setOnlineState(const EOnlineState& value) { mOnlineState = value; }

    inline char getGender() const { return mGender; }
    inline void setGender(char gender) { mGender = gender; }

    bool cacheUserData(const QUrl& folderPath);

Q_SIGNALS:
    void nickNameChanged();
    void avatarChanged();

private:
    unsigned int mUID;

    QString mMD5;

    QString mNickName;

    QString mSignature;

    // 性别。0男，1女，'\0'（空字符）保密
    char mGender;

    QString mAccount;

    QString mPassword;

    QString mAvatar;

    EOnlineState mOnlineState;

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)
};

#endif // USER_H
