#include "MessageItem.h"

#include <MessageDatabase.h>
#include <Utility.h>

#include <QDateTime>
#include <QDir>
#include <QFile>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mReadFlag(false)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    connect(this, &MessageItem::readFlagChanged, [this] {
        MessageDatabase::Instance()->updateReadFlag(this);
    });
    connect(this, &MessageItem::unreadMsgCountChanged, [this] {
        MessageDatabase::Instance()->updateReadFlag(this);
    });
}

MessageItem::~MessageItem()
{
    if (!mChatObject.isNull()) {
        mChatObject.clear();
    }
}
