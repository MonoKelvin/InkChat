#include "MyFriend.h"

#include <Http/HttpRequest.h>
#include <Utility.h>

#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Friend;
}

void MyFriend::fromJson(const QJsonObject& json, bool cache)
{
    mRemark = json.value("remark").toString();
    mSubgroup = json.value("subgroup").toString();

    if (cache) {
        cacheAvatar();
    }

    return IChatObject::fromJson(json, cache);
}

QJsonObject MyFriend::toJson(void)
{
    auto json = IChatObject::toJson();

    json.insert("remark", mRemark);
    json.insert("subgroup", mSubgroup);

    return json;
}
