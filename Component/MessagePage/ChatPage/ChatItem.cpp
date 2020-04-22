#include "ChatItem.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <Utility.h>

#include <QPainter>
#include <Widget/Avatar.h>

ChatItem::ChatItem(QObject* parent)
    : AbstractChatListItem(parent)
    , mSendState(Sending)
    , mChatId(-1)
    , mTime(QDateTime::currentDateTime())
{
}

ChatItem::ChatItem(const ChatItem& item)
    : AbstractChatListItem(item.parent())
    , mSendState(item.mSendState)
    , mChatId(item.mChatId)
    , mTime(item.mTime)
{
    mData = item.mData;
}

ChatItem::~ChatItem()
{
    qDebug() << "Chat Item Destroyed: " << this;
}

void ChatItem::setSendState(const ChatItem::ESendState &sendState)
{
    if (sendState != mSendState)
    {
        mSendState = sendState;
        mTime = QDateTime::currentDateTime();
        emit sendStateChanged();
    }
}

const QString ChatItem::getMessageTime()
{
    return GetMessageTime(mTime);
}

const SUserChatData ChatItem::getSenderData() const noexcept
{
    return mData.value<SUserChatData>();
}

////////////////////////////////////////////////////////////
/// ChatItemDelegate Methods
////////////////////////////////////////////////////////////

void ChatItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& d = index.data(ChatItemDelegate::ChatItemType);
    const auto& chatItem = d.value<ChatItem*>();
    if (chatItem) {
        const auto& item = d.value<AbstractChatListItem*>();
        if (item) {
            item->paintContent(painter, option.rect);
        }
        return;
    } else if (chatItem->selfPaint()) {
        chatItem->paintContent(painter, option.rect);
        return;
    }

    const auto& userData = chatItem->getSenderData();
    const auto& rect = option.rect; // 聊天内容区域
    const int& t = rect.top(); // 每个item需要以此为顶端
    const int& ch = XTheme.AvatarSize / 2; // 头像中心y高度
    QRect avtRect(rect.x(), t, XTheme.AvatarSize, XTheme.AvatarSize); // 头像矩形位置

    painter->save();

    if (userData.Uuid == User::Instance()->getUuid()) {
        avtRect.moveRight(rect.right());

        // 内容最右边位置
        const int& cr = avtRect.x() - ESize::Narrow;

        // 多人聊天才显示名称
        if (!userData.Name.isEmpty()) {
            painter->setPen(XTheme.MainTextColor);
            painter->drawText(QRect(rect.x(), t, cr, ch), Qt::AlignBottom | Qt::AlignRight, userData.Name);
        }

        // 内容控件
        chatItem->paintContent(painter, QRect(rect.x(), t + ch, cr - rect.x(), 1));
    } else {
        // 内容最左边位置
        const int& cl = avtRect.right() + ESize::Narrow;

        if (!userData.Name.isEmpty()) {
            painter->setPen(XTheme.MainTextColor);
            painter->drawText(QRect(cl, t, rect.right(), ch), Qt::AlignBottom | Qt::AlignLeft, userData.Name);
        }

        chatItem->paintContent(painter, QRect(cl, t + ch, rect.right() - cl, 1));
    }

    // 头像
    Avatar::DrawAvatar(painter, avtRect, AppSettings::AvatarCacheFile(userData.Uuid));

    painter->restore();
}

QSize ChatItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& data = index.data(ChatItemDelegate::ChatItemType).value<AbstractChatListItem*>();
    if (!data) {
        return QSize();
    }

    data->updateContentSize(option);
    return QSize(option.rect.width(), qMax(data->getContentSize().height() + ESize::Wide + XTheme.AvatarSize / 2, XTheme.AvatarSize));
}
