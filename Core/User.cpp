#include "User.h"

QMutex User::_mutex;
User *User::UserMe = nullptr;

User::User()
{

}
