#include "MessageItem.h"

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
}

MessageItem::MessageItem(IChatObject* chatObject, QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    Q_ASSERT(chatObject != nullptr);

    mChatObject = chatObject;
    mChatObject->setParent(this);
}
