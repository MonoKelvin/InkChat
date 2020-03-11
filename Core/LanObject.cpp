#include "LanObject.h"

#include <AppSettings.h>

#include <QDir>
#include <QJsonObject>

LanObject::LanObject(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = LAN;
}

void LanObject::fromJson(const QJsonObject& json, bool cache)
{
    mMD5 = json.value(QLatin1String("md5")).toString();
    mNickName = json.value(QLatin1String("remark")).toString();
    mSignature = json.value(QLatin1String("profile")).toString();
    mHostAddress = json.value(QLatin1String("hostAddress")).toString();
    mMacAddress = json.value(QLatin1String("mac")).toString();
    mIsTop = json.value(QLatin1String("top")).toString().toInt();

    if (cache) {
        // lan
    }
}

QJsonObject LanObject::toJson()
{
    QJsonObject json;
    json.insert(QLatin1String("md5"), mMD5);
    json.insert(QLatin1String("top"), mIsTop);
    json.insert(QLatin1String("remark"), mNickName);
    json.insert(QLatin1String("profile"), mSignature);
    json.insert(QLatin1String("mac"), mMacAddress);
    return json;
}

LanObject* LanObject::DetectLanEnvironment()
{
    LanObject* lan = new LanObject;

    const auto addr = getWirelessAddress(&lan->mMacAddress);
    if (addr.isEmpty()) {
        delete lan;
        lan = nullptr;
    } else {
        lan->mMD5 = encryptTextByMD5(lan->mHostAddress + lan->mMacAddress);

        const auto dirList = QDir(AppSettings::LanDataDir()).entryList(QDir::Dirs);

        // 遍历缓存文件夹，如果有缓存就直接读取，否则新建局域网数据文件
        for (auto name : dirList) {
            if (lan->mMD5 == name) {
                QSettings settings(AppSettings::LanSettingsFile(name), QSettings::IniFormat);
                lan->mNickName = settings.value(QString("name")).toString();
                lan->mHostAddress = addr;
                break;
            }
        }

        // 保存数据文件路径
        const QString fileName = AppSettings::LanDataDir() + lan->mMD5;
        if (isFileExists(fileName)) {
        }
    }

    return lan;
}
