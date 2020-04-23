#include "IChatObject.h"

#include <AppSettings.h>
//#include <HttpRequest.h>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QUuid>

IChatObject::IChatObject(QObject* parent)
    : QObject(parent)
    //, mID(0)
    , mRoleType(Stranger)
    , mOnlineState(NoneState)
{
}

const QString IChatObject::getAvatar() const
{
    const auto fileName = AppSettings::AvatarCacheFile(mUuid);
    if (QFileInfo::exists(fileName)) {
        return QStringLiteral("file:///") + fileName;
    }

    return QString();
}

void IChatObject::cacheAvatar(EAvatarSize size)
{
    if (QFileInfo::exists(AppSettings::AvatarCacheFile(mUuid))) {
        return;
    }

    /*HttpRequest* imgRequest = new HttpRequest;
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
    });*/
}

void IChatObject::fromJson(const QJsonObject& json, bool cache)
{
    Q_UNUSED(cache)

    //mID = json.value(QLatin1String("id")).toString().toUInt();
    mMD5 = json.value(QLatin1String("md5")).toString();
    mUuid = json.value(QLatin1String("uuid")).toString();
    mRoleType = ERoleType(json.value(QLatin1String("roleType")).toInt());
    mNickName = json.value(QLatin1String("nickName")).toString();
    mSignature = json.value(QLatin1String("signature")).toString(tr("还没有任何简介"));
    mHostAddress = json.value(QLatin1String("hostAddress")).toString();
}

QJsonObject IChatObject::toJson(void)
{
    QJsonObject json;
    //json.insert(QLatin1String("id"), QString::number(mID));
    json.insert(QLatin1String("md5"), mMD5);
    json.insert(QLatin1String("uuid"), mUuid);
    json.insert(QLatin1String("roleType"), mRoleType);
    json.insert(QLatin1String("nickName"), mNickName);
    json.insert(QLatin1String("signature"), mSignature);
    json.insert(QLatin1String("hostAddress"), mHostAddress);
    return json;
}

const QString IChatObject::generateUuid()
{
    mUuid = QUuid::createUuid().toString(QUuid::Id128);
    return mUuid;
}

const SUserChatData IChatObject::getChatData() noexcept
{
    const SUserChatData data { mUuid, mNickName };
    return data;
}

////////////////////////////////////////////////////
// IPerson Class
////////////////////////////////////////////////////

IPerson::~IPerson()
{
}

void IPerson::fromJson(const QJsonObject& json, bool cache)
{
    mGender = json.value(QLatin1String("gender")).toString(QStringLiteral("-")).front().toLatin1();

    return IChatObject::fromJson(json, cache);
}

QJsonObject IPerson::toJson()
{
    auto json = IChatObject::toJson();
    json.insert(QLatin1String("gender"), QString(mGender));

    return json;
}
