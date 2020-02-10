#ifndef IPERSON_H
#define IPERSON_H

#include <QObject>

class IPerson : public QObject {
    Q_OBJECT

    Q_PROPERTY(unsigned int id READ getID CONSTANT)
    Q_PROPERTY(char gender READ getGender WRITE setGender)
    Q_PROPERTY(QString nickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString signature READ getSignature WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(QString avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(EOnlineState onlineState READ getOnlineState WRITE setOnlineState NOTIFY onlineStateChanged)
    Q_PROPERTY(QString hostAddress READ getHostAddress CONSTANT)

public:
    explicit IPerson(QObject* parent = nullptr);

    enum EOnlineState {
        Online,
        Offline,
        Busy,

        // 不显示状态
        NoneState,
    };
    Q_ENUM(EOnlineState)

    enum EUserType {
        Me,
        Friend,
        Stranger,
        Group,
        BlackList,
        LAN,
    };
    Q_ENUM(EUserType)

    virtual void fromJson(const QJsonObject& json) = 0;
    virtual QJsonObject toJson(void) = 0;

    virtual void cacheData() {}

    inline unsigned int getID(void) const { return mID; }

    inline const QString getNickName(void) const { return mNickName; }
    inline void setNickName(const QString& nickName)
    {
        mNickName = nickName;
        emit nickNameChanged();
    }

    inline QString getSignature(void) const { return mSignature; }
    inline void setSignature(const QString& value)
    {
        mSignature = value;
        emit signatureChanged();
    }

    // todo
    inline const QString getAvatar(void) const { return mAvatar; }
    inline void setAvatar(const QString& value)
    {
        mAvatar = value;
        emit avatarChanged();
    }

    inline char getGender(void) const { return mGender; }
    inline void setGender(char gender)
    {
        mGender = gender;
        emit genderChanged();
    }

    inline EOnlineState getOnlineState(void) const { return mOnlineState; }
    inline void setOnlineState(EOnlineState onlineState)
    {
        mOnlineState = onlineState;
        emit onlineStateChanged();
    }

    inline QString getHostAddress(void) const { return mHostAddress; }

Q_SIGNALS:
    void failed(const QString&);

    void nickNameChanged();
    void signatureChanged();
    void avatarChanged();
    void genderChanged();
    void onlineStateChanged();

protected:
    // 用户ID
    unsigned int mID;

    // 性别。0男，1女，'\0'（空字符）保密
    char mGender;

    // 用户昵称
    QString mNickName;

    // 签名
    QString mSignature;

    // 头像地址
    QString mAvatar;

    // 在线状态
    EOnlineState mOnlineState;

    // 主机地址
    QString mHostAddress;
};

#endif // IPERSON_H
