#include "MyFriend.h"

#include <Http/HttpRequest.h>

#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Friend;
}

void MyFriend::fromJson(const QJsonObject& json, bool cache)
{
    mRemark = json.value(QLatin1String("remark")).toString();
    mSubgroup = json.value(QLatin1String("subgroup")).toString();

    if (cache) {
        cacheAvatar();
    }

    return IChatObject::fromJson(json, cache);
}

QJsonObject MyFriend::toJson(void)
{
    auto json = IChatObject::toJson();

    json.insert(QLatin1String("remark"), mRemark);
    json.insert(QLatin1String("subgroup"), mSubgroup);

    return json;
}
