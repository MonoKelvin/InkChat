#ifndef ICHATOBJECT_H
#define ICHATOBJECT_H

#include <QObject>

/**
 * @brief 聊天对象基类接口
 */
class IChatObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(unsigned int id READ getID CONSTANT)
    Q_PROPERTY(bool isTop READ getIsTop WRITE setIsTop NOTIFY isTopChanged)
    Q_PROPERTY(char gender READ getGender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY(QString nickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString signature READ getSignature WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(QString avatar READ getAvatar CONSTANT)
    Q_PROPERTY(EOnlineState onlineState READ getOnlineState WRITE setOnlineState NOTIFY onlineStateChanged)
    Q_PROPERTY(QString hostAddress READ getHostAddress CONSTANT)
    Q_PROPERTY(ERoleType roleType READ getRoleType CONSTANT)

public:
    explicit IChatObject(QObject* parent = nullptr);
    virtual ~IChatObject();

    /**
     * @brief 更新标志，用于区分哪些数据需要进行更新
     */
    /*enum EUpdateFlag {
        UF_MD5 = 0x00000001,
        UF_Gender = 0x00000002,
        UF_IsTop = 0x00000004,
        UF_RoleType = 0x00000008,
        UF_NickName = 0x00000010,
        UF_Signature = 0x00000020,
        UF_OnlineState = 0x00000040,
        UF_HostAddress = 0x00000080,
        UF_CustomFlag1 = 0x00000100,
        UF_CustomFlag2 = 0x00000200,
        UF_CustomFlag3 = 0x00000400,
        UF_CustomFlag4 = 0x00000800,
        UF_CustomFlag5 = 0x00001000,
        UF_CustomFlag6 = 0x00002000,
        UF_CustomFlag7 = 0x00004000,
        UF_CustomFlag8 = 0x00008000,
        UF_CustomFlag9 = 0x00010000,
        UF_CustomFlag10 = 0x00020000,
    };
    Q_ENUM(EUpdateFlag)*/

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
        SinglePerson = Me | Friend | Stranger,
        MultiPerson = LAN | Group,
        // BlackList,
        AllUser = Me | Friend | Stranger | LAN | Group,
    };
    Q_ENUM(ERoleType)

    /** 头像尺寸枚举 */
    enum EAvatarSize {
        AvatarSize45x45 = 45,
        AvatarSize64x64 = 64,
        AvatarSize128x128 = 128,
        AvatarSize256x256 = 256,
        AvatarSize512x512 = 512,

        AvatarSizeThumb = AvatarSize45x45,
        AvatarSizeMax = AvatarSize512x512
    };
    Q_ENUM(EAvatarSize)

    inline unsigned int getID(void) const { return mID; }
    inline void setID(unsigned int id) { mID = id; }

    inline const QString getMD5(void) const { return mMD5; }
    inline void setMD5(const QString& md5) { mMD5 = md5; }

    inline bool getIsTop() const { return mIsTop; }
    inline void setIsTop(bool isTop)
    {
        if (isTop != mIsTop) {
            mIsTop = isTop;
            emit isTopChanged();
        }
    }

    inline const QString getNickName(void) const { return mNickName; }
    inline void setNickName(const QString& nickName)
    {
        if (nickName != mNickName) {
            mNickName = nickName;
            emit nickNameChanged();
        }
    }

    inline QString getSignature(void) const { return mSignature; }
    inline void setSignature(const QString& signature)
    {
        if (signature != mSignature) {
            mSignature = signature;
            emit signatureChanged();
        }
    }

    /**
     * @brief 获得本地头像文件路径
     * @return 如果文件存在返回true，否则返回空字符串
     */
    virtual const QString getAvatar(void) const;

    inline char getGender(void) const { return mGender; }
    inline void setGender(char gender)
    {
        if (gender != mGender) {
            mGender = gender;
            emit genderChanged();
        }
    }

    inline EOnlineState getOnlineState(void) const { return mOnlineState; }
    inline void setOnlineState(EOnlineState onlineState)
    {
        if (onlineState != mOnlineState) {
            mOnlineState = onlineState;
            emit onlineStateChanged();
        }
    }

    inline void setHostAddress(const QString& address) { mHostAddress = address; }
    inline const QString getHostAddress() { return mHostAddress; }

    inline void setRoleType(const ERoleType& type) { mRoleType = type; }
    inline ERoleType getRoleType(void) const { return mRoleType; }

    /**
     * @brief 解析json数据，一般是来自数据库的数据
     * @param json json对象
     * @param cache 是否缓存数据
     */
    virtual void fromJson(const QJsonObject& json, bool cache = true);

    /**
     * @brief 将数据转换成json格式
     * @param fetchIfNull 如果数据为空就先从数据库中获取，然后再返回，如果获取失败则返回空Json对象
     * @return QJsonObject json对象
     */
    virtual QJsonObject toJson(/*bool fetchIfNull = true*/);

    // TODO: 加密文件方法
    //typedef bool (*EncryptFileFunc)(class QFile*);
    // TODO: 解密文件方法
    //typedef const QByteArray (*DecryptFileFunc)(QFile*);

    /**
     * @brief 更新本地数据
     * @return 更新成功返回true，否则返回false。默认返回true
     */
    virtual bool updateLocalData() { return true; }

protected Q_SLOTS:
    /**
     * @brief 从数据库中获取对象的头像并缓存
     * @param size 保存的尺寸 @see EAvatarSize
     * @note 缓存或失败都会发送信号 @see avatarCached。
     * @note 默认保存在/data/${user_id}/cache/avatar/下
     */
    virtual void cacheAvatar(EAvatarSize size = AvatarSizeThumb);

Q_SIGNALS:
    /** 信号：当头像缓存（可能缓存失败）后发送的信号
     * @param 缓存成功ok = true，否则为false
     * @param 一些消息或日志
     */
    void avatarCached(bool ok, const QString& msg);

    /** 信号：数据需要更新时触发，会调用 @see updateJson方法 */
    //void needToUpdate();

    void isTopChanged();
    void nickNameChanged();
    void signatureChanged();
    void avatarChanged();
    void genderChanged();
    void onlineStateChanged();

    //protected:
    //    /**
    //     * @brief 加载缓存数据
    //     * @return 加成功返回true，否则返回false
    //     */
    //    virtual bool loadCache(void) { return true; }

protected:
    // 用户ID
    unsigned int mID;

    // 数据完整性校验
    QString mMD5;

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

    // 在线状态
    EOnlineState mOnlineState;

    // 主机地址，如果是局域网对象则保存的应该是局域网地址
    QString mHostAddress;
};

#endif // ICHATOBJECT_H
