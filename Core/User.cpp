#include "User.h"

#include <AbstractChatListItem.h>
#include <AppSettings.h>
#include <LanObject.h>
#include <MessageManager.h>
#include <MyFriend.h>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

User::User(QObject* parent)
    : IPerson(parent)
{
    mRoleType = ERoleType::Me;
}

User::~User()
{
    for (const auto& i : mMyChatObjects) {
        if (i->getRoleType() & MultiPerson) {
            MessageManager::Instance()->sendUserBehavior(i->getHostAddress(), AbstractChatListItem::UserLeft);
        }
    }

    qDebug() << "User Destroyed: " << this;
    mMyChatObjects.clear();
}

void User::fromJson(const QJsonObject& json, bool cache)
{
    IChatObject::fromJson(json, cache);

    mAccount = json.value(QLatin1String("account")).toString();
    mPassword = json.value(QLatin1String("password")).toString();

    if (cache) {
        const auto fileName = AppSettings::UserDataFile();
        if (!IsFileExists(fileName, true)) {
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

bool User::setAvatar(const QString& fileName)
{
    CopyFile(fileName, AppSettings::UserDataDir() + QStringLiteral("avatar"));
    return IPerson::setAvatar(fileName);
}

LanObject* User::getLanObjectByUuid(const QString& uuid)
{
    return static_cast<LanObject*>(getChatObjectByUuid(uuid));
}

IChatObject* User::getChatObjectByUuid(const QString& uuid)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getUuid() == uuid) {
            return mMyChatObjects[i];
        }
    }

    return dynamicLoadCacheData(uuid);
}

IChatObject* User::dynamicLoadCacheData(const QString& uuid)
{
    QFile file(AppSettings::ChatObjectCacheFile(uuid));

    // 动态加载
    IChatObject* chatObj = nullptr;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QJsonParseError err;
        const auto& doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            // TODO: add more type
            switch (doc.object().value(QLatin1String("roleType")).toInt()) {
            case LAN:
                chatObj = new LanObject(this);
                break;
            case Friend:
                chatObj = new MyFriend(this);
                break;
            }

            if (chatObj) {
                chatObj->fromJson(doc.object());
                mMyChatObjects.append(chatObj);
            }
        }
    }
    file.close();

    return chatObj;
}
