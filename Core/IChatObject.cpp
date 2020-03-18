#include "IChatObject.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>

IChatObject::IChatObject(QObject* parent)
    : QObject(parent)
    , mID(0)
    , mGender('-')
    , mIsTop(false)
    , mRoleType(Stranger)
    , mOnlineState(NoneState)
{
}

IChatObject::~IChatObject()
{
    qDebug() << "ChatObject Destroyed: " << this;
}

const QString IChatObject::getAvatar() const
{
    const auto fileName = AppSettings::AvatarCacheFile(mRoleType, mID);
    if (QFileInfo::exists(fileName)) {
        return QStringLiteral("file:///") + fileName;
    }

    return QStringLiteral("");
}

void IChatObject::cacheAvatar(EAvatarSize size)
{
    if (QFileInfo::exists(AppSettings::AvatarCacheFile(mRoleType, mID))) {
        return;
    }

    HttpRequest* imgRequest = new HttpRequest;
    imgRequest->sendRequest(AppSettings::UserAvatarUrl(mID, size));

    QObject::connect(imgRequest, &HttpRequest::request, [this](bool success, const QByteArray& data) {
        if (success) {
            QPixmap pixmap;
            if (pixmap.loadFromData(data)) {
                pixmap.save(AppSettings::AvatarCacheFile(mRoleType, mID), "PNG");
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

    mID = json.value(QLatin1String("id")).toString().toUInt();
    mMD5 = json.value(QLatin1String("md5")).toString();
    mIsTop = json.value(QLatin1String("top")).toBool(false);
    mGender = json.value(QLatin1String("gender")).toString(QStringLiteral("-")).front().toLatin1();
    mNickName = json.value(QLatin1String("nickName")).toString();
    mSignature = json.value(QLatin1String("signature")).toString(tr("还没有任何简介"));
    mHostAddress = json.value(QLatin1String("hostAddress")).toString();
}

QJsonObject IChatObject::toJson(void)
{
    QJsonObject json;
    json.insert(QLatin1String("top"), mIsTop);
    json.insert(QLatin1String("id"), QString::number(mID));
    json.insert(QLatin1String("md5"), mMD5);
    json.insert(QLatin1String("gender"), QString(mGender));
    json.insert(QLatin1String("nickName"), mNickName);
    json.insert(QLatin1String("signature"), mSignature);
    json.insert(QLatin1String("hostAddress"), mHostAddress);
    return json;
}
