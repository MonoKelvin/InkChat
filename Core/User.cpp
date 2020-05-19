#include "User.h"

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
            MessageManager::Instance()->sendMessage(i, AbstractChatListItem::UserLeft);
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
            file.close();
            throw tr("用户数据文件打开失败！");
        }

        QJsonDocument jsonDoc(json);
        if (file.write(jsonDoc.toJson(QJsonDocument::Compact).toBase64()) == -1) {
            file.close();
            throw tr("用户数据文件写入失败！");
        }

        file.close();
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

IChatObject* User::getChatObjectByUuid(const QString& uuid, bool createIfNull)
{
    for (int i = 0; i < mMyChatObjects.size(); i++) {
        if (mMyChatObjects.at(i)->getUuid() == uuid) {
            return mMyChatObjects[i];
        }
    }

    return dynamicLoadCacheData(uuid, createIfNull);
}

void User::updateLocalData()
{
    const auto& fileName = AppSettings::UserDataFile();
    if (!IsFileExists(fileName, true)) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QJsonDocument jsonDoc(toJson());
        file.write(jsonDoc.toJson(QJsonDocument::Compact));
    }
    file.close();
}

IChatObject* User::dynamicLoadCacheData(const QString& uuid, bool createIfNull)
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

    if (createIfNull && chatObj == nullptr) {
        chatObj = new IChatObject(this);
        chatObj->setUuid(uuid);
        mMyChatObjects.append(chatObj);
    }

    return chatObj;
}
