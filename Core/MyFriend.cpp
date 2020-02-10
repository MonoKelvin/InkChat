#include "MyFriend.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>
#include <Utility.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : IPerson(parent)
{
}

void MyFriend::fromJson(const QJsonObject& json)
{
    mIsTop = json.value("top").toBool();
    mRemark = json.value("remark").toString();
    mSubgroup = json.value("subgroup").toString();

    return IPerson::fromJson(json);
}

QJsonObject MyFriend::toJson()
{
    auto json = IPerson::toJson();

    json.insert("top", mIsTop);
    json.insert("remark", mRemark);
    json.insert("subgroup", mSubgroup);

    return json;
}
