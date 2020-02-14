#include "IChatObject.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>
#include <QJsonObject>

#include <QPixmap>

IChatObject::IChatObject(QObject* parent)
    : QObject(parent)
    , mGender('\0')
    , mOnlineState(NoneState)
{
}

IChatObject::~IChatObject()
{
}

const QString IChatObject::getAvatar() const
{
    return AppPaths::AvatarCacheDir(mMD5);
}

void IChatObject::setAvatar(const QString& value)
{
}

void IChatObject::cacheAvatar(EAvatarSize size)
{
    HttpRequest* imgRequest = new HttpRequest;
    imgRequest->sendRequest(AppPaths::UserAvatarUrl(size));

    QObject::connect(imgRequest, &HttpRequest::request, [this](bool success, const QByteArray& data) {
        if (success) {
            QPixmap pixmap;
            if (pixmap.loadFromData(data)) {
                pixmap.save(AppPaths::AvatarCacheDir(mMD5), "PNG");
                emit avatarCached(true, QStringLiteral(""));
            } else {
                emit avatarCached(false, QStringLiteral("FEIL_PRASE_FAILED: VALUE=avatar"));
            }
        }

        emit avatarCached(false, QStringLiteral("DATA_CACHE_FAILED: VALUE=avatar"));
    });
}

void IChatObject::fromJson(const QJsonObject& json, bool cache)
{
    Q_UNUSED(cache)

    mID = json.value(QStringLiteral("id")).toString().toUInt();
    mMD5 = json.value(QStringLiteral("md5")).toString();
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
    json.insert(QStringLiteral("md5"), mMD5);
    json.insert(QStringLiteral("gender"), QString(mGender));
    json.insert(QStringLiteral("nickName"), mNickName);
    json.insert(QStringLiteral("signature"), mSignature);
    json.insert(QStringLiteral("hostAddress"), mHostAddress);
    return json;
}
