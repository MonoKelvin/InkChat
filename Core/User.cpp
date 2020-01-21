#include "User.h"

std::mutex User::_mutex;
std::mutex User::_mutex2;
User *User::UserMe = nullptr;

User::User()
{

}
