#include "LanObject.h"

#include <AppSettings.h>
#include <User.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

LanObject::LanObject(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = LAN;
}

void LanObject::fromJson(const QJsonObject& json, bool cache) noexcept(false)
{
    IChatObject::fromJson(json, cache);

    mMacAddress = json.value(QLatin1String("mac")).toString();

    if (cache) {
        const auto fileName = AppSettings::LanDataDir() + mMD5;
        if (!isFileExists(fileName, true)) {
            throw tr("局域网数据文件创建失败");
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

    return json;
}

bool LanObject::updateLocalData()
{
    const auto fileName = AppSettings::LanDataDir() + mMD5;

    if (!isFileExists(fileName, true)) {
        return false;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QJsonDocument jsonDoc(toJson());
        if (file.write(jsonDoc.toJson(QJsonDocument::Compact)) == -1) {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

LanObject* LanObject::DetectLanEnvironment()
{
    QString mac, netName;
    const QString& addr = getWirelessAddress(&mac);

    // 未检测到局域网环境就返回nullptr
    if (addr.isEmpty()) {
        return nullptr;
    }

    const auto& md5 = encryptTextByMD5(addr + mac);

    // 先从缓存中获取
    LanObject* lan = User::Instance()->getLanObjectByMd5(md5);

    // 检测到一个新的局域网
    if (nullptr == lan) {
        lan = new LanObject;

        // WARINING: 必须加到用户列表，否则刷新消息视图将出现重复对象
        User::Instance()->addChatObject(lan);

        lan->mMD5 = md5;
        lan->mNickName = addr;
        lan->mHostAddress = addr;
        lan->mMacAddress = mac;
        lan->updateLocalData();
    }

    return lan;
}
