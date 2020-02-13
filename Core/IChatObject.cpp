#include "IChatObject.h"

#include <QDebug>
#include <QJsonObject>

IChatObject::IChatObject(QObject* parent)
    : QObject(parent)
    , mGender('\0')
    , mOnlineState(NoneState)
{
}

IChatObject::~IChatObject()
{
}

void IChatObject::fromJson(const QJsonObject& json)
{
    mID = json.value(QStringLiteral("id")).toString().toUInt();
    mIsTop = json.value("top").toBool();
    mGender = json.value(QStringLiteral("gender")).toString("-").front().toLatin1();
    mNickName = json.value(QStringLiteral("nickName")).toString();
    mSignature = json.value(QStringLiteral("signature")).toString();
    mHostAddress = json.value(QStringLiteral("hostAddress")).toString();
}

QJsonObject IChatObject::toJson(void)
{
    QJsonObject json;
    json.insert("top", mIsTop);
    json.insert(QStringLiteral("id"), QString::number(mID));
    json.insert(QStringLiteral("gender"), mGender);
    json.insert(QStringLiteral("nickName"), mNickName);
    json.insert(QStringLiteral("signature"), mSignature);
    json.insert(QStringLiteral("hostAddress"), mHostAddress);
    return json;
}
