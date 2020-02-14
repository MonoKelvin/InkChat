#include "MessageItem.h"

#include <MyFriend.h>
#include <User.h>
#include <Utility.h>

#include <QDir>
#include <QFile>
#include <QJsonDocument>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    connect(this, &MessageItem::destroyed, [this] {
        if (mDirty) {
            update();
        }
    });
}

MessageItem::~MessageItem()
{
    SAFE_DELETE(mChatObject);
}

void MessageItem::setUnreadMsgCount(int count)
{
    if (count != mUnreadMsgCount) {
        mDirty = true;
        mUnreadMsgCount = count;
        emit unreadMsgCountChanged();
    }
}

bool MessageItem::load(const QFileInfo& fileInfo)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc;
    QFile crFile(fileInfo.filePath());

    if (!crFile.open(QFile::ReadOnly | QFile::Text)) {
        crFile.close();
        throw QStringLiteral("FILE_OPEN_FAILED: ") + fileInfo.filePath();
    }

    // TODO: 解密

    jsonDoc = QJsonDocument::fromJson(crFile.readAll(), &jsonError);
    crFile.close();

    if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        throw QStringLiteral("DATA_FORMAT_ERROR: ") + jsonError.errorString();
    }

    mJsonObject = jsonDoc.object();

    if (!mJsonObject.value(QStringLiteral("chat")).toBool(false)) {
        return false;
    }

    // ID数据是否和文件名匹配，匹配说明数据可能为未篡改
    const auto id = unsigned(mJsonObject.value(QStringLiteral("id")).toDouble(0));
    if (fileInfo.baseName() != encryptTextByMD5(QString::number(id), true)) {
        throw QStringLiteral("DATA_MISMATCH: VALUE=id");
    }

    // 读取发送者
    const auto roleType = mJsonObject.value(QStringLiteral("roleType")).toInt(-1);
    switch (roleType) {
    case IChatObject::Friend:
        mChatObject = User::Instance()->getFriendById(mJsonObject.value("id").toString().toUInt());
        break;
    default:
        throw QStringLiteral("CHAT_OBJECT_INVALID: VALUE=%1").arg(roleType);
    }

    mTime = mJsonObject.value(QStringLiteral("lastTime")).toString();
    mMessage = mJsonObject.value(QStringLiteral("lastMsg")).toString();
    mReadFlag = mJsonObject.value(QStringLiteral("readFlag")).toBool(true);
    mUnreadMsgCount = mJsonObject.value(QStringLiteral("unreadMsgCount")).toInt(0);

    update();

    return true;
}

void MessageItem::update()
{
    if (mJsonObject.isEmpty()) {
        return;
    }
}
