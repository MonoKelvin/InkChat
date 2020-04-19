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
    : QObject(item.parent())
    , mSendState(item.mSendState)
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

    // 聊天内容区域
    const auto& rect = option.rect;
    const int& t = rect.top(); // 每个item需要以此为顶端
    const bool& isMultiPerson = itemData->getSender()->getRoleType() & IChatObject::MultiPerson;
    QRect avtRect(rect.x(), t, AVATAR_SIZE, AVATAR_SIZE);
    QRect ctRect;
    const int& ch = AVATAR_SIZE / 2; // 头像中心y高度

    painter->save();

    if (itemData->getSender()->getRoleType() == IChatObject::Me) {
        avtRect.moveRight(rect.right());

        // 多人聊天才显示名称
        if (isMultiPerson) {
            const QString& name = itemData->getSender()->getNickName();
            const QRect nameRect(rect.x(), t, avtRect.x() - ESpacing::Narrow, ch);
            painter->setPen(QPen("#2D3135"));
            painter->drawText(nameRect, Qt::AlignBottom | Qt::AlignRight, name);
        }

        // 内容控件
        itemData->paintContent(painter, QRect(rect.x(), t + ch, avtRect.x() - ESpacing::Narrow - rect.x(), 1));
        ctRect = itemData->contentArea();

        // 时间
        const QString& time = itemData->getMessageTime();
        const QRect timeRect(ctRect.x(), ctRect.bottom() + ESpacing::Tiny, qMax(50, ctRect.width()), 20);
        painter->setPen(QPen("#A7ADBD"));
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, time);
    } else {
        if (isMultiPerson) {
            const QString& name = itemData->getSender()->getNickName();
            const QRect nameRect(avtRect.right() + ESpacing::Narrow, t, rect.right(), ch);
            painter->setPen(QPen("#2D3135"));
            painter->drawText(nameRect, Qt::AlignBottom | Qt::AlignLeft, name);
        }

        itemData->paintContent(painter, QRect(avtRect.right() + ESpacing::Narrow, t + ch, rect.right() - avtRect.right() - ESpacing::Narrow, 1));
        ctRect = itemData->contentArea();

        const QString& time = itemData->getMessageTime();
        const QRect timeRect(ctRect.x(), ctRect.bottom() + ESpacing::Tiny, qMax(50, ctRect.width()), 20);
        painter->setPen(QPen("#A7ADBD"));
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, time);
    }

    // 头像
    Avatar::DrawAvatar(painter, avtRect, itemData->getSender()->getAvatar());

    painter->restore();
}

QSize ChatItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& itemData = index.data(ChatItemDelegate::ChatItemType).value<IChatItem*>();
    if (!itemData) {
        return QSize();
    }

    return QSize(option.rect.width(), qMax(itemData->contentArea().height() + ESpacing::Wide + AVATAR_SIZE / 2, AVATAR_SIZE));
}
