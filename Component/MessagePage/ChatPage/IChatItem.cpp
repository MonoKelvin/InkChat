#include "IChatItem.h"

#include <MyFriend.h>
#include <User.h>
#include <Utility.h>

#include <QDebug>
#include <QVariantMap>

IChatItem::IChatItem(QObject* parent)
    : QObject(parent)
    , mSendState(Sending)
    , mChatId(-1)
    , mTime(QDateTime::currentDateTime())
{
}

IChatItem::IChatItem(const IChatItem& item)
    : mSendState(item.mSendState)
    , mChatId(item.mChatId)
    , mTime(item.mTime)
    , mChatObject(item.mChatObject)
{
}

IChatItem::~IChatItem()
{
    qDebug() << "Chat Item Destroyed: " << this;
    if (mChatObject) {
        mChatObject.clear();
    }
}

void IChatItem::setSendState(const IChatItem::ESendState& sendState)
{
    if (sendState != mSendState) {
        mSendState = sendState;
        mTime = QDateTime::currentDateTime();
        emit sendStateChanged();
    }
}

const QString IChatItem::getMessageTime()
{
    return GetMessageTime(mTime);
}
