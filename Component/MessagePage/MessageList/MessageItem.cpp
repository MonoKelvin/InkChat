#include "MessageItem.h"

//#include <MessageDatabase.h>
#include <Utility.h>

#include <QDateTime>
#include <QDir>
#include <QFile>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mDirty(false)
    , mReadFlag(false)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    //    connect(this, &MessageItem::destroyed, [this] {
    //        if (mDirty) {
    //        }
    //    });
}

MessageItem::~MessageItem()
{
    if (!mChatObject.isNull()) {
        mChatObject.clear();
    }
}

void MessageItem::setUnreadMsgCount(int count)
{
    if (count != mUnreadMsgCount) {
        mUnreadMsgCount = count;
        emit unreadMsgCountChanged();
    }
}
