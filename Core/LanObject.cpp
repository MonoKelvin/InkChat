#include "LanObject.h"

#include <AppSettings.h>

#include <QJsonObject>

LanObject::LanObject(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = LAN;
}

void LanObject::fromJson(const QJsonObject& json, bool cache)
{
    mID = json.value(QLatin1String("id")).toString().toUInt();
    mMD5 = json.value(QLatin1String("md5")).toString();
    mNickName = json.value(QLatin1String("remark")).toString();
    mSignature = json.value(QLatin1String("profile")).toString();
    mHostAddress = json.value(QLatin1String("hostAddress")).toString();
    mMacAddress = json.value(QLatin1String("mac")).toString();
    mIsTop = json.value(QLatin1String("top")).toBool();

    if (cache) {
        // lan
    }
}

QJsonObject LanObject::toJson()
{
    QJsonObject json;
    json.insert(QLatin1String("id"), QString::number(mID));
    json.insert(QLatin1String("md5"), mMD5);
    json.insert(QLatin1String("top"), mIsTop);
    json.insert(QLatin1String("remark"), mNickName);
    json.insert(QLatin1String("profile"), mSignature);
    json.insert(QLatin1String("mac"), mMacAddress);
    return json;
}
