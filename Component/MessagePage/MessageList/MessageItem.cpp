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
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    connect(this, &MessageItem::readFlagChanged, [this] {
        MessageDatabase::Instance()->updateReadFlag(this);
    });
    connect(this, &MessageItem::unreadMsgCountChanged, [this] {
        MessageDatabase::Instance()->updateUnreadMsgCount(this);
    });
}

MessageItem::~MessageItem()
{
    if (!mChatObject.isNull()) {
        mChatObject.clear();
    }

    qDebug() << "MessageItem Destroyed" << this;
}

void MessageItem::setChatObject(QSharedPointer<IChatObject> chatObject)
{
    if (mChatObject) {
        if (mChatObject == chatObject) {
            return;
        } else {
            disconnect(mChatObject.get(), &IChatObject::isTopChanged, this, &MessageItem::onTopChanged);
        }
    } else if (chatObject) {
        mChatObject.clear();
        mChatObject = chatObject;
        connect(mChatObject.get(), &IChatObject::isTopChanged, this, &MessageItem::onTopChanged);
    }
}

void MessageItem::onTopChanged()
{
    // 如果在线登录，就更新远程数据库，否则更新本地数据
    if (AppSettings::OfflineUserName.isEmpty()) {
        const auto postData = QStringLiteral("uid=%1&fid=%2&top=%3")
                                  .arg(AppSettings::Instance()->getCurrentUserId())
                                  .arg(mChatObject->getID())
                                  .arg(mChatObject->getIsTop());

        HttpRequest* request = new HttpRequest;

        // 发送请求，无论是否成功都继续执行
        request->sendRequest(UpdateFriendUrl, HttpRequest::POST, postData);
    } else {
        mChatObject->updateLocalData();
    }
}
