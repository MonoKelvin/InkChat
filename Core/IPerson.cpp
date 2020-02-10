#include "IPerson.h"

#include <QJsonObject>

IPerson::IPerson(QObject* parent)
    : QObject(parent)
    , mGender('\0')
    , mOnlineState(NoneState)
{
}

void IPerson::fromJson(const QJsonObject& json)
{
    mID = json.value(QStringLiteral("id")).toString().toUInt();
    mGender = json.value(QStringLiteral("gender")).toString()[0].toLatin1();
    mNickName = json.value(QStringLiteral("nickName")).toString();
    mSignature = json.value(QStringLiteral("signature")).toString();
    mHostAddress = json.value(QStringLiteral("hostAddress")).toString();
}

QJsonObject IPerson::toJson(void)
{
    QJsonObject json;
    json.insert(QStringLiteral("id"), QString::number(mID));
    json.insert(QStringLiteral("gender"), mGender);
    json.insert(QStringLiteral("nickName"), mNickName);
    json.insert(QStringLiteral("signature"), mSignature);
    json.insert(QStringLiteral("hostAddress"), mHostAddress);
    return json;
}
