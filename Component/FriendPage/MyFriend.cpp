#include "MyFriend.h"

#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : QObject(parent)
{
}

void MyFriend::fromJson(const QJsonObject& json)
{
    UID = json.value("id").toString().toUInt();
    Gender = json.value("gender").toString()[0].toLatin1();
    IsTop = json.value("isTop").toBool();
    Remark = json.value("remark").toString();
    Signature = json.value("signature").toString();
    Thumb = json.value("thumb").toString();
    Subgroup = json.value("subgroup").toString();
    HostAddress = json.value("hostAddress").toString();
}

QJsonObject MyFriend::toJson()
{
    QJsonObject jsonObj;

    jsonObj.insert("id", QString::number(UID));
    jsonObj.insert("gender", Gender);
    jsonObj.insert("isTop", IsTop);
    jsonObj.insert("remark", Remark);
    jsonObj.insert("signature", Signature);
    jsonObj.insert("thumb", Thumb);
    jsonObj.insert("subgroup", Subgroup);
    jsonObj.insert("hostAddress", HostAddress);

    return jsonObj;
}
