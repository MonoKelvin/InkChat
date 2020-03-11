#include "MessageItem.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <MessageDatabase.h>

#include <QDateTime>
#include <QDir>
#include <QFile>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mReadFlag(false)
    , mUnreadMsgCount(1)
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

void MessageItem::setChatObject(QSharedPointer<IChatObject> chatObject)
{
    Q_ASSERT(chatObject.get() != mChatObject.get());

    mChatObject = chatObject;
    connect(mChatObject.get(), &IChatObject::isTopChanged, this, &MessageItem::onTopChanged);
}

void MessageItem::onTopChanged()
{
    const auto postData = QStringLiteral("uid=%1&fid=%2&top=%3")
                              .arg(AppSettings::Instance()->getCurrentUserId())
                              .arg(mChatObject->getID())
                              .arg(mChatObject->getIsTop());

    HttpRequest* request = new HttpRequest;

    // 发送请求，无论是否成功都继续执行
    request->sendRequest(UpdateFriendUrl, HttpRequest::POST, postData);
}
