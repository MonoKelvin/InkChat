#include "MessageItem.h"

#include <QJsonObject>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
    , mJsonObject(nullptr)
{
    connect(this, &MessageItem::destroyed, [this] {
        if (mDirty) {
            cache();
        }
    });
}

MessageItem::MessageItem(IChatObject* chatObject, QJsonObject* jsonObject, QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
    , mJsonObject(jsonObject)
{
    Q_ASSERT(chatObject != nullptr);

    mChatObject = chatObject;
    mChatObject->setParent(this);

    mTime = mJsonObject->value(QStringLiteral("lastTime")).toString();
    mMessage = mJsonObject->value(QStringLiteral("lastMsg")).toString();
    mReadFlag = mJsonObject->value(QStringLiteral("readFlag")).toBool(true);
    mUnreadMsgCount = mJsonObject->value(QStringLiteral("unreadMsgCount")).toInt(0);

    connect(this, &MessageItem::destroyed, [this] {
        if (mDirty) {
            cache();
        }
        delete mJsonObject;
        mChatObject = nullptr;
    });
}

MessageItem::~MessageItem()
{
}

void MessageItem::setUnreadMsgCount(int count)
{
    if (count != mUnreadMsgCount) {
        mDirty = true;
        mUnreadMsgCount = count;
        emit unreadMsgCountChanged();
    }
}

void MessageItem::cache()
{
}

void MessageItem::update()
{
    if (!mJsonObject) {
        return;
    }
}
