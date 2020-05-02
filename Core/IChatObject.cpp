#include "IChatObject.h"

#include <AppSettings.h>

#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

IChatObject::IChatObject(QObject* parent)
    : QObject(parent)
    //, mID(0)
    , mRoleType(Stranger)
    , mOnlineState(NoneState)
{
}

const QString IChatObject::getAvatar() const noexcept
{
    return AppSettings::AvatarCacheFile(mUuid);
}

bool IChatObject::setAvatar(const QString& fileName)
{
    QPixmap pix(fileName);
    if (pix.isNull()) {
        return false;
    }

    IsDirExists(AppSettings::UserDir() + QStringLiteral("/Avatar"), true);
    const auto& thumb = pix.scaled(AvatarSizeThumb, AvatarSizeThumb, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if (thumb.save(AppSettings::AvatarCacheFile(mUuid), "JPG", 99)) {
        emit avatarChanged();
        return true;
    }
    return false;
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
    return json;
}

void IChatObject::updateLocalData()
{
    const auto& fileName = AppSettings::ChatObjectCacheFile(mUuid);
    if (!IsFileExists(fileName, true)) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QJsonDocument jsonDoc(toJson());
        file.write(jsonDoc.toJson(QJsonDocument::Compact));
    }
    file.close();
}

const QString IChatObject::generateUuid()
{
    mUuid = QUuid::createUuid().toString(QUuid::Id128);
    return mUuid;
}

const SUserBaseData IChatObject::getChatData(const QVariant& msg) noexcept
{
    return { mUuid, mNickName, msg };
}

bool IChatObject::selectAvatarFile()
{
    QFileDialog fd;
    fd.setWindowTitle(tr("选择头像"));
    fd.setNameFilter(tr("图片(*.png;*.jpg;*.jpeg)"));
    fd.setViewMode(QFileDialog::Detail);

    if (fd.exec()) {
        const auto& files = fd.selectedFiles();
        if (files.length() > 0 && setAvatar(files.at(0))) {
            return true;
        }
    }
    return false;
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
