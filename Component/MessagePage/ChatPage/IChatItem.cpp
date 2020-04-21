#include "IChatItem.h"

#include <AppTheme.h>
#include <MyFriend.h>
#include <User.h>
#include <Utility.h>
#include <Widget/Avatar.h>

#include <QPainter>
#include <QVariantMap>

IChatItem::IChatItem(QObject *parent)
    : QObject(parent), mSendState(Sending), mChatId(-1), mTime(QDateTime::currentDateTime())
{
}

IChatItem::IChatItem(const IChatItem &item)
    : QObject(item.parent()), mSendState(item.mSendState), mChatId(item.mChatId), mTime(item.mTime), mChatObject(item.mChatObject)
{
}

IChatItem::~IChatItem()
{
    qDebug() << "Chat Item Destroyed: " << this;
    if (mChatObject)
    {
        mChatObject.clear();
    }
}

void IChatItem::setSendState(const IChatItem::ESendState &sendState)
{
    if (sendState != mSendState)
    {
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

void ChatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto &data = index.data(ChatItemDelegate::ChatItemType).value<IChatItem *>();
    if (!data) {
        return;
    } else if (data->selfPaint()) {
        data->paintContent(painter, option.rect);
        return;
    }

    // 聊天内容区域
    const auto &rect = option.rect;
    const int &t = rect.top(); // 每个item需要以此为顶端
    const bool &isMultiPerson = data->getSender()->getRoleType() & IChatObject::MultiPerson;
    QRect avtRect(rect.x(), t, XTheme.AvatarSize, XTheme.AvatarSize); // 头像矩形位置
    const auto &ctSize = data->getContentSize();                      // 内容大小
    const int &ch = XTheme.AvatarSize / 2;                            // 头像中心y高度

    painter->save();

    if (data->getSender()->getRoleType() == IChatObject::Me)
    {
        avtRect.moveRight(rect.right());

        // 内容最右边位置
        const int &cr = avtRect.x() - ESize::Narrow;

        // 多人聊天才显示名称
        if (isMultiPerson)
        {
            painter->setPen(XTheme.MainTextColor);
            painter->drawText(QRect(rect.x(), t, cr, ch), Qt::AlignBottom | Qt::AlignRight, data->getSender()->getNickName());
        }

        // 内容控件
        data->paintContent(painter, QRect(rect.x(), t + ch, cr - rect.x(), 1));

        // 时间
        const QRect timeRect(cr - ctSize.width(), t + ch + ctSize.height() + ESize::Tiny, qMax(50, ctSize.width()), 20);
        painter->setPen(XTheme.SubTextColor);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, data->getMessageTime());
    }
    else
    {
        // 内容最左边位置
        const int &cl = avtRect.right() + ESize::Narrow;

        if (isMultiPerson)
        {
            painter->setPen(XTheme.MainTextColor);
            painter->drawText(QRect(cl, t, rect.right(), ch), Qt::AlignBottom | Qt::AlignLeft, data->getSender()->getNickName());
        }

        data->paintContent(painter, QRect(cl, t + ch, rect.right() - cl, 1));

        const QRect timeRect(cl, t + ch + ctSize.height() + ESize::Tiny, qMax(50, ctSize.width()), ESize::Wide);
        painter->setPen(XTheme.SubTextColor);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, data->getMessageTime());
    }

    // 头像
    Avatar::DrawAvatar(painter, avtRect, data->getSender()->getAvatar());

    painter->restore();
}

QSize ChatItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto &data = index.data(ChatItemDelegate::ChatItemType).value<IChatItem *>();
    if (!data)
    {
        return QSize();
    }

    data->updateContentSize(option);
    return QSize(option.rect.width(), qMax(data->getContentSize().height() + ESize::Wide + XTheme.AvatarSize / 2, XTheme.AvatarSize));
}
