#include "MessageItem.h"

#include <AppSettings.h>
#include <AppTheme.h>
#include <MessageDatabase.h>
#include <Widget/Avatar.h>

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
        const auto itemData = index.data(MessageItemRole).value<MessageItem*>();
        // item 矩形区域
        const QRect& rect = option.rect;

        painter->save();
        painter->setPen(Qt::NoPen);

        // 鼠标移过或置顶
        if (option.state.testFlag(QStyle::State_MouseOver) || itemData->mChatObject->getIsTop()) {
            painter->setBrush(XTheme.TintColor);
            painter->drawRect(rect);
        }

        // 选中
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setBrush(XTheme.PrimayColor1);
            painter->drawRect(0, 0, 4, rect.height());
        }

        // 置顶标识
        if (itemData->mChatObject->getIsTop()) {
            QPainterPath path;
            path.moveTo(rect.topRight());
            path.lineTo(rect.topRight() - QPointF(10, 0));
            path.lineTo(rect.topRight() + QPointF(0, 10));
            path.lineTo(rect.topRight());
            painter->setBrush(XTheme.TintColor);
            painter->drawPath(path);
        }

        // 绘制头像
        const QRect avtRect(ESize::Narrow, (rect.height() - XTheme.AvatarSize) / 2, XTheme.AvatarSize, XTheme.AvatarSize);
        Avatar::DrawAvatar(painter, avtRect, itemData->mChatObject->getAvatar(), itemData->mChatObject->getOnlineState());

        // 字体
        painter->setFont(XTheme.StdFont);

        const int ch = avtRect.center().y(); // 矩形中心线高度
        const int timeWidth = getFontPixelWidth(XTheme.StdFont, itemData->mTime); //时间字体的宽度

        // 绘制消息数量
        QRect mcRect(rect.width(), ch, 0, 0);
        if (!itemData->mReadFlag) {
            const QString msgCountStr = QString::number(itemData->mUnreadMsgCount);

            mcRect.setTop(ch);
            mcRect.setRight(rect.width() - ESize::Narrow);
            mcRect.setHeight(getFontPixelHeight(XTheme.StdFont));
            mcRect.setWidth(getFontPixelWidth(XTheme.StdFont, msgCountStr) + ESize::Narrow);

            painter->setPen(Qt::white);
            painter->setBrush(XTheme.PrimayColor1);
            painter->drawText(mcRect, Qt::AlignCenter, msgCountStr);
            painter->drawRoundedRect(mcRect, int((float(mcRect.width()) / mcRect.height()) * 100), 100);
        }

        // 绘制时间
        const QRect timeRect(rect.width() - ESize::Std - timeWidth, ESize::Std, timeWidth, ch - ESize::Std);
        painter->setPen(XTheme.SubTextColor);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(timeRect, Qt::AlignHCenter | Qt::AlignBottom, itemData->mTime);

        // 绘制消息概要
        QRect msgRect;
        const QString& msg = itemData->mMessage.isEmpty() ? tr("[暂无最近消息]") : itemData->mMessage;
        msgRect.setLeft(avtRect.right() + ESize::Narrow);
        msgRect.setTop(ch);
        msgRect.setRight(mcRect.left() + ESize::Std);
        msgRect.setBottom(rect.height() - ESize::Std);
        painter->drawText(msgRect, Qt::AlignLeft | Qt::AlignTop, getElidedText(msg, XTheme.StdFont, msgRect.width()));

        // 绘制名称
        QRect nameRect;
        nameRect.setLeft(msgRect.left());
        nameRect.setRight(timeRect.left() + ESize::Std);
        nameRect.setTop(ESize::Std);
        nameRect.setBottom(ch);
        painter->setFont(XTheme.SubTitleFont);
        painter->setPen(XTheme.MainTextColor);
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignBottom, itemData->mChatObject->getNickName());

        painter->restore();
    }
}
