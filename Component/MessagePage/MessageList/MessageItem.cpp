#include "MessageItem.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <MessageDatabase.h>
#include <Widget/Avatar.h>

MessageItem::MessageItem(QObject* parent)
    : QObject(parent)
    , mIsTop(false)
    , mReadFlag(true)
    , mUnreadMsgCount(0)
    , mChatObject(nullptr)
{
    connect(
        this, &MessageItem::readFlagChanged, this, [this] {
            MessageDatabase::Instance()->updateReadFlag(this);
        },
        Qt::QueuedConnection);
    connect(
        this, &MessageItem::unreadMsgCountChanged, this, [this] {
            MessageDatabase::Instance()->updateUnreadMsgCount(this);
        },
        Qt::QueuedConnection);
    connect(
        this, &MessageItem::topChanged, this, [this] {
            MessageDatabase::Instance()->updateTop(this);
        },
        Qt::QueuedConnection);
}

MessageItem::~MessageItem()
{
    if (mChatObject) {
        mChatObject.clear();
    }
    qDebug() << "MessageItem Destroyed" << this;
}

void MessageItem::setChatObject(IChatObject* chatObject)
{
    Q_ASSERT(chatObject);

    mChatObject.clear();
    mChatObject = chatObject;
}

void MessageItem::setTime(const QDateTime& time) noexcept
{
    mTime = GetMessageTime(time);
}

void MessageItem::onTopChanged()
{
    // 如果在线登录，就更新远程数据库，否则更新本地数据
    /*if (AppSettings::OfflineUserName.isEmpty()) {
        const auto postData = QStringLiteral("uid=%1&fid=%2&top=%3")
                                  .arg(AppSettings::Instance()->getCurrentUserId())
                                  .arg(mChatObject->getID())
                                  .arg(mChatObject->getIsTop());

    HttpRequest* request = new HttpRequest;
    
    // 发送请求，无论是否成功都继续执行
    request->sendRequest(UpdateFriendUrl, HttpRequest::POST, postData);
} else {
    mChatObject->updateLocalData();
}*/

    mChatObject->updateLocalData();
}

//------------------------------------------------------------------------------
// MessageItemDelegate
//------------------------------------------------------------------------------

QSize MessageItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)

    return QSize(option.rect.width(), 70);
}

void MessageItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        // item 数据
        const auto& itemData = index.data(MessageItemRole).value<MessageItem*>();
        // item 矩形区域
        const auto& rect = option.rect;
        const int& t = rect.top();

        painter->save();
        painter->setPen(Qt::NoPen);

        // 鼠标移过或置顶
        if (option.state.testFlag(QStyle::State_MouseOver) || itemData->mIsTop) {
            painter->setBrush(XTheme.TintColor);
            painter->drawRect(rect);
        }

        // 选中
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setBrush(XTheme.PrimayColor1);
            painter->drawRect(0, t, 4, rect.height());
        }

        // 置顶标识
        if (itemData->mIsTop) {
            QPainterPath path;
            path.moveTo(rect.topRight());
            path.lineTo(rect.topRight() - QPointF(10, 0));
            path.lineTo(rect.topRight() + QPointF(0, 10));
            path.lineTo(rect.topRight());
            painter->setBrush(XTheme.PrimayColor1);
            painter->drawPath(path);
        }

        // 绘制头像
        const QRect avtRect(ESize::Narrow, t + (rect.height() - XTheme.AvatarSize) / 2, XTheme.AvatarSize, XTheme.AvatarSize);
        Avatar::DrawAvatar(painter, avtRect, itemData->mChatObject->getAvatar(), itemData->mChatObject->getNickName(), itemData->mChatObject->getOnlineState());

        // 字体
        painter->setFont(XTheme.StdFont);

        const int& ch = avtRect.center().y(); // 矩形中心线高度

        // 绘制消息数量
        QRect mcRect(rect.width(), ch, 0, 0);
        if (!itemData->mReadFlag) {
            const auto& msgCountStr = QString::number(itemData->mUnreadMsgCount);

            mcRect.setWidth(GetFontPixelWidth(XTheme.StdFont, msgCountStr) + ESize::Std);
            mcRect.setHeight(qMin(18, mcRect.width()));
            mcRect.moveTop(ch + ESize::Tiny);
            mcRect.moveRight(rect.right() - ESize::Narrow);

            // 背景
            painter->setPen(Qt::NoPen);
            painter->setBrush(XTheme.PrimayColor1);
            painter->setRenderHint(QPainter::Antialiasing);
            painter->drawRoundedRect(mcRect, int(float(mcRect.height()) / mcRect.width() * 100), 100, Qt::RelativeSize);
            painter->setRenderHint(QPainter::Antialiasing, false);

            // 文字
            painter->setPen(Qt::white);
            painter->drawText(mcRect, Qt::AlignCenter, msgCountStr);
        }

        // 绘制时间
        const int& tw = GetFontPixelWidth(XTheme.StdFont, itemData->mTime); //时间字体的宽度
        const QRect timeRect(rect.right() - ESize::Narrow - tw, t, tw, ch - t);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(XTheme.SubTextColor);
        painter->drawText(timeRect, Qt::AlignHCenter | Qt::AlignBottom, itemData->mTime);

        // 绘制消息概要
        const QRect msgRect(avtRect.right() + ESize::Narrow, ch, mcRect.left() - avtRect.right() - ESize::Narrow, ch - t);
        painter->drawText(msgRect, Qt::AlignLeft | Qt::AlignTop, GetElidedText(itemData->mMessage, XTheme.StdFont, msgRect.width()));

        // 绘制名称
        const QRect nameRect(msgRect.left(), t, timeRect.left() - msgRect.left() - ESize::Std, ch - t);
        painter->setFont(XTheme.SubTitleFont);
        painter->setPen(XTheme.MainTextColor);
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignBottom, itemData->mChatObject->getNickName());

        painter->restore();
    }
}
