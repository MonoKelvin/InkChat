#include "IChatItem.h"

#include <Utility.h>

#include <QVariantMap>

IChatItem::IChatItem(QObject* parent)
    : QObject(parent)
    , mSendState(Sending)
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
    mChatObject.clear();
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

void IChatItem::unpackage(QVariantMap& data)
{
    mChatId = data.value(QStringLiteral("id")).toUInt();
    mTime = data.value(QStringLiteral("time")).toDateTime();
}

void IChatItem::package(QVariantMap& data)
{
    data.insert(QStringLiteral("id"), mChatId);
    data.insert(QStringLiteral("time"), mTime);
}
