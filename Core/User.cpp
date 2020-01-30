#include "User.h"

#include "Widget/Avatar.h"

std::mutex User::_mutex;
std::mutex User::_mutex2;
User *User::UserMe = nullptr;

User::User()
{

}

User::~User()
{
}

QSharedPointer<User> User::GetUnloginUser(const QString &name)
{
    QSharedPointer<User> user = QSharedPointer<User>(new User);

    user->NickName = name;
    user->Avatar = Avatar::GetDefaultPixmap();

    return user;
}
