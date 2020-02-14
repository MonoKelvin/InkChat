#include "User.h"

#include <AppSettings.h>
#include <MyFriend.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QMutex User::mMutex(QMutex::Recursive);
User* User::mInstance = nullptr;
User::_GarbageCollection User::_GC;

User::User(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Me;
}

User::~User()
{
    qDeleteAll(mFriends);
    mFriends.clear();
}

void User::fromJson(const QJsonObject& json, bool cache)
{
    IChatObject::fromJson(json);

    mAccount = json.value(QStringLiteral("account")).toString();
    mPassword = json.value(QStringLiteral("password")).toString();

    QJsonArray friends = json.value(QStringLiteral("friends")).toArray();
    for (const auto i : friends) {
        MyFriend* f = new MyFriend();
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

    json.insert(QStringLiteral("account"), QJsonValue(mAccount));
    json.insert(QStringLiteral("password"), QJsonValue(mPassword));

    QJsonArray friends;
    for (const auto i : mFriends) {
        friends.append(i->toJson());
    }
    json.insert(QStringLiteral("friends"), friends);

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

bool User::loadCache()
{
    // 当前登录的用户
    mID = AppPaths::GetCurrentUserId();
    if (mID == 0) {
        return false;
    }

    const auto fileName = AppPaths::UserDataFile();

    // 如果有缓存就加载缓存数据，否则从数据库中获取
    if (isFileExists(fileName)) {
        QFile file(fileName);
        try {
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                throw "FILE_READ_FAILED: " + fileName;
            }

            // TODO: 解密、校验数据是否被篡改

            auto jsonDoc = QJsonDocument::fromJson(file.readAll());
            if (jsonDoc.isNull() || !jsonDoc.isObject()) {
                throw "FILE_PARSE_FAILED: " + fileName;
            } else {
                fromJson(jsonDoc.object());
                return true;
            }
        } catch (const QString& msg) {
            file.close();
            // TODO: 日志
            qDebug() << msg;
        }
    }

    return false;
}
