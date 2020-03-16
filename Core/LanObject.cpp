#include "LanObject.h"

#include <AppSettings.h>

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

    if (!cache) {
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

LanObject* LanObject::DetectLanEnvironment()
{
    QString mac, netName;
    const QString& addr = getWirelessAddress(&mac);

    // 未检测到局域网环境就返回nullptr
    if (addr.isEmpty()) {
        return nullptr;
    }

    const auto& md5 = encryptTextByMD5(addr + mac);
    const auto& fileNames = QDir(AppSettings::LanDataDir()).entryList(QDir::Files);

    // 检测到局域网并检查是否有缓存，有则加载缓存数据
    for (auto i : fileNames) {
        if (i == md5) {
            QFile file(AppSettings::LanDataDir() + i);
            // 文件打开失败
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                break;
            }

            // 文件解析失败
            QJsonParseError jsonErr;
            const auto jsonDoc = QJsonDocument::fromJson(file.readAll(), &jsonErr);
            if (jsonErr.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
                break;
            }

            LanObject* lan = new LanObject;
            lan->fromJson(jsonDoc.object());

            return lan;
        }
    }

    // 有局域网但缓存不存在，则新建一个
    LanObject* lan = new LanObject;

    QJsonObject json;
    json.insert(QLatin1String("md5"), md5);
    json.insert(QLatin1String("hostAddress"), addr);
    json.insert(QLatin1String("mac"), mac);
    json.insert(QLatin1String("top"), false);

    lan->fromJson(json);
    return lan;
}
