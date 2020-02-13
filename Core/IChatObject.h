#ifndef ICHATOBJECT_H
#define ICHATOBJECT_H

#include <QObject>

class IChatObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(unsigned int id READ getID CONSTANT)
    Q_PROPERTY(bool isTop READ getIsTop WRITE setIsTop NOTIFY isTopChanged)
    Q_PROPERTY(char gender READ getGender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY(QString nickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString signature READ getSignature WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(QString avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(EOnlineState onlineState READ getOnlineState WRITE setOnlineState NOTIFY onlineStateChanged)
    Q_PROPERTY(QString hostAddress READ getHostAddress CONSTANT)

public:
    explicit IChatObject(QObject* parent = nullptr);
    virtual ~IChatObject();

    enum EOnlineState {
        Online,
        Offline,
        Busy,

        // 不显示状态
        NoneState,
    };
    Q_ENUM(EOnlineState)

    enum ERoleType {
        Me = 0x0001,
        Friend = 0x0002,
        Stranger = 0x0004,
        LAN = 0x0008,
        Group = 0x0010,
        // BlackList,
        AllUser = Me | Friend | Stranger | LAN | Group,
    };
    Q_ENUM(ERoleType)

    virtual void fromJson(const QJsonObject& json) = 0;
    virtual QJsonObject toJson(void) = 0;

    inline unsigned int getID(void) const { return mID; }
    inline void setID(unsigned int id) { mID = id; }

    inline bool getIsTop() const { return mIsTop; }
    inline void setIsTop(bool isTop)
    {
        mIsTop = isTop;
        emit isTopChanged();
    }

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

    inline ERoleType getRoleType(void) const { return mRoleType; }
    inline void setRoleType(ERoleType roleType) { mRoleType = roleType; }

public Q_SLOTS:
    virtual bool cacheData() { return true; }
    virtual bool loadData() { return true; }

Q_SIGNALS:
    void failed(const QString&);

    void isTopChanged();
    void nickNameChanged();
    void signatureChanged();
    void avatarChanged();
    void genderChanged();
    void onlineStateChanged();

protected:
    // 用户ID
    unsigned int mID;

    // 性别。1男，0女，'-'（减号）保密或无（组和局域网）
    char mGender;

    // 是否置顶
    bool mIsTop;

    // 角色类型
    ERoleType mRoleType;

    // 昵称
    QString mNickName;

    // 签名或简介
    QString mSignature;

    // 头像地址
    QString mAvatar;

    // 在线状态
    EOnlineState mOnlineState;

    // 主机地址
    QString mHostAddress;
};

#endif // ICHATOBJECT_H
