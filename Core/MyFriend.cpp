#include "MyFriend.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>
#include <Utility.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : IChatObject(parent)
{
    mRoleType = ERoleType::Friend;
}

void MyFriend::fromJson(const QJsonObject& json)
{
    mRemark = json.value("remark").toString();
    mSubgroup = json.value("subgroup").toString();

    return IChatObject::fromJson(json);
}

QJsonObject MyFriend::toJson()
{
    auto json = IChatObject::toJson();

    json.insert("remark", mRemark);
    json.insert("subgroup", mSubgroup);

    return json;
}
