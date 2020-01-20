#ifndef USER_H
#define USER_H

#include <QString>
#include <QPixmap>


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

    static QSharedPointer<User> GetMe(void);

private:
    Q_DISABLE_COPY(User)
    Q_DISABLE_MOVE(User)

    unsigned int mUID;
    QString mNickName;
    QPixmap mAvatar;
//    QString mPassword;
    QString mAccount;
};

#endif // USER_H
