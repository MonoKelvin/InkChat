#ifndef USER_H
#define USER_H

#include <QString>
#include <QPixmap>
#include <thread>
#include <mutex>

class User
{
public:
    explicit User();

    enum EOnlineState
    {
        Online,
        Offline,
        Busy,
        None,
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

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    unsigned int mUID;

    QString mNickName;

    QString mMD5;

    QString mAccount;

    QPixmap mAvatar;

private:
    static std::mutex _mutex;
    static std::mutex _mutex2;

    static User *UserMe;
};

#endif // USER_H
