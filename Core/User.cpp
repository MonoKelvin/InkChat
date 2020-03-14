#include "User.h"

#include <AppSettings.h>
#include <LanObject.h>
#include <MyFriend.h>

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
    mMyChatObjects.clear();
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
        mMyChatObjects.append(f);
    }

    if (cache) {
        const auto fileName = AppSettings::UserDataFile();
        if (!isFileExists(fileName, true)) {
            throw tr("用户文件创建失败");
        }

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

    // 写入好友数据
    QJsonArray friends;
    for (const auto i : mMyChatObjects) {
        if (i->getRoleType() == Friend) {
            friends.append(i->toJson());
        }
    }
    json.insert(QLatin1String("friends"), friends);

    return json;
}

MyFriend* User::getFriendById(unsigned int id)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getID() == id
            && mMyChatObjects.at(i)->getRoleType() == Friend) {
            return static_cast<MyFriend*>(mMyChatObjects[i]);
        }
    }

    return nullptr;
}

LanObject* User::getLanObjectById(unsigned int id)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getID() == id
            && mMyChatObjects.at(i)->getRoleType() == LAN) {
            return static_cast<LanObject*>(mMyChatObjects[i]);
        }
    }

    return nullptr;
}

LanObject* User::getLanObjectByMd5(const QString& md5)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getMD5() == md5
            && mMyChatObjects.at(i)->getRoleType() == LAN) {
            return static_cast<LanObject*>(mMyChatObjects[i]);
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
