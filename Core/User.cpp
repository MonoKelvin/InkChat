#include "User.h"

#include <AppSettings.h>
#include <LanObject.h>
#include <MyFriend.h>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

User::User(QObject* parent)
    : IPerson(parent)
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

    return json;
}

/*IChatObject* User::getChatObjectById(unsigned int id)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getID() == id) {
            return mMyChatObjects[i];
        }
    }

    IChatObject* obj = nullptr;

    dynamicLoadCacheData(obj, AppSettings::ChatObjectCacheFile(id));

    return obj;
}*/

IChatObject* User::getChatObjectByUuid(const QString& uuid)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getUuid() == uuid) {
            return mMyChatObjects[i];
        }
    }

    IChatObject* obj = nullptr;

    dynamicLoadCacheData(obj, AppSettings::ChatObjectCacheFile(uuid));

    return obj;
}

LanObject* User::getLanObjectByUuid(const QString& uuid)
{
    return static_cast<LanObject*>(getChatObjectByUuid(uuid));
}

void User::dynamicLoadCacheData(IChatObject* chatObj, const QString& fileName)
{
    QFile file(fileName);

    // 动态加载
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QJsonParseError err;
        const auto& doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error != QJsonParseError::NoError && doc.isObject()) {
            chatObj = new LanObject(this);
            chatObj->fromJson(doc.object());
            mMyChatObjects.append(chatObj);
        }
    }
    file.close();
}

bool User::hasCache()
{
    // 当前登录的用户
    mID = AppSettings::Instance()->getCurrentUserId();
    if (mID == 0) {
        return false;
    }

    return false;
}
