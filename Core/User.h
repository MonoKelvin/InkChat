#ifndef USER_H
#define USER_H

#include <QPixmap>
#include <thread>
#include <mutex>

class User
{
public:
    explicit User();
    ~User();

    enum EOnlineState
    {
        Online,
        Offline,
        Busy,

        // 不显示左下角的状态
        NoneState,
    };

    static User *GetMe(void)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (UserMe)
        {
            std::lock_guard<std::mutex> lock(_mutex2);
            if (UserMe)
            {
                UserMe = new User();
            }
        }
        return UserMe;
    }

    static void DestroyMe(void)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (UserMe)
        {
            std::lock_guard<std::mutex> lock(_mutex2);
            if (UserMe)
            {
                delete UserMe;
                UserMe = nullptr;
            }
        }
    }

    static QSharedPointer<User> GetUnloginUser(const QString &name = QObject::tr("未登录"));

    void setID(unsigned int id) { mUID = id; }
    void setNickName(const QString &name) { mNickName = name; }
    void setMD5(const QString &md5) { mMD5 = md5; }
    void setAccount(const QString &account) { mAccount = account; }
    void setAvatar(const QPixmap &pixmap) { mAvatar = pixmap; }

    inline unsigned int getUID(void) const { return mUID; }
    inline const QString getNickName(void) const { return mNickName; }
    inline const QPixmap getAvatar(void) const { return mAvatar; }
    inline EOnlineState getOnlineState(void) const { return mOnlineState; }

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    unsigned int mUID;

    QString mNickName;

    QString mMD5;

    QString mAccount;

    QPixmap mAvatar;

    EOnlineState mOnlineState;

private:
    static std::mutex _mutex;
    static std::mutex _mutex2;

    static User *UserMe;
};

#endif // USER_H
