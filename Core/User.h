#ifndef USER_H
#define USER_H

#include <QPixmap>
#include <thread>
#include <mutex>

class User : public QObject
{
    Q_OBJECT

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
    Q_ENUM(EOnlineState)

    enum EUserType
    {
        Friend,
        Stranger,
        Group,
        BlackList,
    };
    Q_ENUM(EUserType)

    static User *GetMe(void)
    {
        static std::mutex _mutex;

        std::lock_guard<std::mutex> lock(_mutex);
        if (UserMe)
        {
            static std::mutex _mutex2;
            std::lock_guard<std::mutex> lock(_mutex2);
            if (UserMe)
            {
                // todo: 获取数据库数据
                UserMe = new User();
            }
        }
        return UserMe;
    }

    static void DestroyMe(void)
    {
        static std::mutex _mutex;

        std::lock_guard<std::mutex> lock(_mutex);
        if (UserMe)
        {
            static std::mutex _mutex2;
            std::lock_guard<std::mutex> lock(_mutex2);
            if (UserMe)
            {
                delete UserMe;
                UserMe = nullptr;
            }
        }
    }

    static QSharedPointer<User> GetUnloginUser(const QString &name = QObject::tr("未登录"));

public:
    unsigned int UID;

    QString NickName;

    QString Signature;

    QString MD5;

    QString Account;

    QString Password;

    QPixmap Avatar;

    EOnlineState OnlineState;

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    static User *UserMe;
};

#endif // USER_H
