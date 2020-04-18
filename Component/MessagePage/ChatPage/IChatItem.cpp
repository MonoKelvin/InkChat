#include "IChatItem.h"

#include <Configuation.h>
#include <MyFriend.h>
#include <User.h>
#include <Utility.h>
#include <Widget/Avatar.h>

#include <QPainter>
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

/////////////////////////////////////////////
// ChatItem Delegate
/////////////////////////////////////////////

ChatItemDelegate::ChatItemDelegate(QWidget* parent)
    : QAbstractItemDelegate(parent)
{
}

ChatItemDelegate::~ChatItemDelegate()
{
}

//void ChatItemDelegate::setChatItemData(IChatItem* data)
//{
//    Q_ASSERT(data != nullptr);

//    auto widget = data->contentWidget();
//    Q_ASSERT(widget != nullptr);

//    itemData.reset(data);
//    widget->setParent(this);
//    if (itemData->getSender()->getRoleType() == IChatObject::Me) {
//        widget->setObjectName(QStringLiteral("chatItem_me"));
//    } else {
//        widget->setObjectName(QStringLiteral("chatItem_ta"));
//    }
//}

void ChatItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& itemData = index.data(ChatItemDelegate::ChatItemType).value<IChatItem*>();
    if (!itemData) {
        return;
    }

    // 聊天内容尺寸
    QRect ctRect;

    const auto& rw = option.rect.width();
    const auto& avatar = itemData->getSender()->getAvatar();
    const bool& isMultiPerson = itemData->getSender()->getRoleType() & IChatObject::MultiPerson;

    painter->save();
    QRect avtRect(0, 0, AVATAR_SIZE, AVATAR_SIZE);
    if (itemData->getSender()->getRoleType() == IChatObject::Me) {
        avtRect.setX(rw - AVATAR_SIZE - ESpacing::Narrow);

        // 多人聊天才显示名称
        if (isMultiPerson) {
            const QString& name = itemData->getSender()->getNickName();
            const QRect nameRect(0, 0, rw - avtRect.left() - ESpacing::Narrow, AVATAR_SIZE / 2);
            painter->setPen(QPen("#2D3135"));
            painter->drawText(nameRect, Qt::AlignBottom | Qt::AlignRight, name);
        }

        // 内容控件
        ctRect = itemData->paintContent(painter, QRect(0, AVATAR_SIZE / 2, rw - avtRect.left() - ESpacing::Narrow, 0));

        // 时间
        const QString& time = itemData->getMessageTime();
        QRect timeRect;
        timeRect.setX(ctRect.x());
        timeRect.setY(ctRect.bottom() + ESpacing::Tiny);
        timeRect.setWidth(qMax(50, ctRect.width()));
        timeRect.setHeight(20);
        painter->setPen(QPen("#A7ADBD"));
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, time);
    } else {
        avtRect.setX(ESpacing::Narrow);
        if (isMultiPerson) {
            const QString& name = itemData->getSender()->getNickName();
            const QRect rect(avtRect.right() + ESpacing::Narrow, 0, rw, AVATAR_SIZE / 2);
            painter->setPen(QPen("#2D3135"));
            painter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, name);
        }

        ctRect = itemData->paintContent(painter, QRect(avtRect.right() + ESpacing::Narrow, AVATAR_SIZE / 2, rw - avtRect.right() - ESpacing::Narrow, 0));

        const QString& time = itemData->getMessageTime();
        QRect timeRect;
        timeRect.setX(ctRect.x());
        timeRect.setY(ctRect.bottom() + ESpacing::Tiny);
        timeRect.setWidth(qMax(50, ctRect.width()));
        timeRect.setHeight(20);
        painter->setPen(QPen("#A7ADBD"));
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, time);
    }

    // 头像
    Avatar::DrawAvatar(painter, avtRect, avatar);
    painter->restore();

    mCurrentSize = QSize(rw, qMax(ctRect.bottom() + 25, AVATAR_SIZE));
}

QSize ChatItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return mCurrentSize;
}
