#include "LanObject.h"

#include <QJsonObject>

LanObject::LanObject(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = LAN;
}

void LanObject::fromJson(const QJsonObject& json, bool cache)
{
    IChatObject::fromJson(json, cache);

    if (cache) {
        cacheAvatar();
    }
}

QJsonObject LanObject::toJson()
{
    return IChatObject::toJson();
}
