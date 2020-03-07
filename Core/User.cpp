#include "User.h"

#include <AppSettings.h>
#include <LanObject.h>
#include <MyFriend.h>
#include <Utility.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

User::User(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Me;
    mHostAddress = getWirelessAddress();
}

User::~User()
{
    mFriends.clear();
    mLANs.clear();

    qDebug() << "User Model Destroyed";
}

void User::fromJson(const QJsonObject& json, bool cache)
{
    IChatObject::fromJson(json, cache);

    mAccount = json.value(QLatin1String("account")).toString();
    mPassword = json.value(QLatin1String("password")).toString();

    // 加载好友
    QJsonArray chatObjs = json.value(QLatin1String("friends")).toArray();
    for (const auto i : chatObjs) {
        MyFriend* f = new MyFriend(this);
        f->fromJson(i.toObject(), cache);
        addFriend(f);
    }

    // 加载局域网聊天群
    chatObjs = json.value(QLatin1String("lans")).toArray();
    for (const auto i : chatObjs) {
        LanObject* lan = new LanObject(this);
        lan->fromJson(i.toObject(), cache);
        addLanObject(lan);
    }

    if (cache) {
        const auto fileName = AppSettings::UserDataFile();
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

        cacheAvatar(AvatarSizeMax);
    }
}

QJsonObject User::toJson()
{
    auto json = IChatObject::toJson();

    json.insert(QLatin1String("account"), QJsonValue(mAccount));
    json.insert(QLatin1String("password"), QJsonValue(mPassword));

    // 好友
    QJsonArray friends;
    for (const auto i : mFriends) {
        friends.append(i->toJson());
    }
    json.insert(QLatin1String("friends"), friends);

    // 局域网
    QJsonArray lans;
    for (const auto i : mLANs) {
        lans.append(i->toJson());
    }
    json.insert(QLatin1String("lans"), lans);

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

LanObject* User::getLanObjectById(unsigned int id)
{
    for (int i = 0; i < mLANs.size(); i++) {
        if (mLANs.at(i)->getID() == id) {
            return mLANs[i];
        }
    }

    return nullptr;
}

bool User::hasCache()
{
    // 当前登录的用户
    mID = AppSettings::Instance()->getCurrentUserId();
    if (mID == 0) {
        return false;
    }

    const auto fileName = AppSettings::UserDataFile();

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
