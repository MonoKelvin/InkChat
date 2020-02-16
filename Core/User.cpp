#include "User.h"

#include <AppSettings.h>
#include <MyFriend.h>
#include <Utility.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QMutex User::mMutex(QMutex::Recursive);
QPointer<User> User::mInstance = nullptr;
User::_GarbageCollection User::_GC;

User::User(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Me;
}

User::~User()
{
    mFriends.clear();
}

void User::fromJson(const QJsonObject& json, bool cache)
{
    IChatObject::fromJson(json);

    mAccount = json.value(QLatin1String("account")).toString();
    mPassword = json.value(QLatin1String("password")).toString();

    QJsonArray friends = json.value(QLatin1String("friends")).toArray();
    for (const auto i : friends) {
        MyFriend* f = new MyFriend(this);
        f->fromJson(i.toObject());
        addFriend(f);
    }

    if (cache) {
        const auto fileName = AppPaths::UserDataFile();
        isFileExists(fileName, true); // 总是返回true

        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            throw tr("用户数据文件打开失败！");
        }

        // TODO: ARS加密文件、MD5生成文件名

        QJsonDocument jsonDoc(json);
        if (file.write(jsonDoc.toJson(QJsonDocument::Indented /*Compact*/)) == -1) {
            throw tr("用户数据文件写入失败！");
        }
    }
}

QJsonObject User::toJson()
{
    auto json = IChatObject::toJson();

    json.insert(QLatin1String("account"), QJsonValue(mAccount));
    json.insert(QLatin1String("password"), QJsonValue(mPassword));

    QJsonArray friends;
    for (const auto i : mFriends) {
        friends.append(i->toJson());
    }
    json.insert(QLatin1String("friends"), friends);

    return json;
}

QQmlListProperty<MyFriend> User::getFriends()
{
    return QQmlListProperty<MyFriend>(this, this->mFriends);
}

MyFriend* User::getFriendById(unsigned int id)
{
    for (int i = 0; i < mFriends.size(); i++) {
        if (mFriends.at(i)->getID() == id) {
            return mFriends[i];
        }
    }

    return nullptr;
}

void User::addFriend(MyFriend* myFriend)
{
    mFriends.append(myFriend);
}

bool User::hasCache()
{
    // 当前登录的用户
    mID = AppPaths::GetCurrentUserId();
    if (mID == 0) {
        return false;
    }

    const auto fileName = AppPaths::UserDataFile();

    if (isFileExists(fileName)) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            // TODO: 解密

            auto jsonDoc = QJsonDocument::fromJson(file.readAll());
            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                const auto json = jsonDoc.object();
                mAccount = json.value(QLatin1String("account")).toString();
                mPassword = json.value(QLatin1String("password")).toString();
                return true;
            }
        }
    }

    return false;
}
