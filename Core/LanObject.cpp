#include "LanObject.h"

#include <AbstractChatListItem.h>
#include <AppSettings.h>
#include <MessageDatabase.h>
#include <User.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

LanObject::LanObject(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = LAN;
}

LanObject::~LanObject()
{
    qDebug() << "LanObject Destroyed: " << this;
    mMembers.clear();
}

void LanObject::fromJson(const QJsonObject& json, bool cache) noexcept(false)
{
    IChatObject::fromJson(json, cache);

    mMacAddress = json.value(QLatin1String("mac")).toString();

    if (cache) {
        const auto fileName = AppSettings::ChatObjectCacheFile(mUuid);
        if (!IsFileExists(fileName, true)) {
            throw tr("局域网缓存数据创建失败");
        }

        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            throw tr("局域网数据文件打开失败！");
        }

        // 不需要加解密

        QJsonDocument jsonDoc(json);
        if (file.write(jsonDoc.toJson(QJsonDocument::Compact)) == -1) {
            throw tr("局域网数据文件缓存失败！");
        }
    }
}

QJsonObject LanObject::toJson()
{
    auto json = IChatObject::toJson();

    json.insert(QLatin1String("mac"), mMacAddress);
    json.insert(QLatin1String("hostAddress"), mHostAddress);

    return json;
}

void LanObject::setMemberBehavior(int type, const SUserBaseData& userData)
{
    const int& index = mMembers.indexOf(userData);
    switch (type) {
    case AbstractChatListItem::UserJoin:
        if (index == -1) {
            mMembers.append(userData);
        }
        break;
    case AbstractChatListItem::UserLeft:
        if (index >= 0) {
            mMembers.removeAt(index);
        }
        break;
    }
}

LanObject* LanObject::DetectLanEnvironment()
{
    QString mac;
    const QString& addr = GetWirelessAddress(&mac);

    // 未检测到局域网环境就返回nullptr
    if (addr.isEmpty()) {
        return nullptr;
    }

    const auto& md5_16 = EncryptTextByMD5(addr + mac, true);
    LanObject* lan = MessageDatabase::Instance()->getCachedLanObject(md5_16);

    // 先从缓存中获取，不存在就新建一个局域网
    if (nullptr == lan) {
        lan = new LanObject;

        lan->mMD5 = md5_16;
        lan->mNickName = addr;
        lan->mHostAddress = addr;
        lan->mMacAddress = mac;

        // WARINING: 必须加到用户列表，否则刷新消息视图将出现重复对象
        User::Instance()->addChatObject(lan);
    } else {
        lan->mMD5 = md5_16;
        lan->mMacAddress = mac;
        lan->mHostAddress = addr;
    }

    return lan;
}
