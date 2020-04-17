#include "MessageItem.h"

#include <AppSettings.h>
#include <Configuation.h>
#include <MessageDatabase.h>
#include <Utility.h>
#include <Widget/Avatar.h>

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

QSize MessageItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)

    return QSize(option.rect.width(), 70);
}

void MessageItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();
        painter->setPen(Qt::NoPen);

        // item 矩形区域
        const QRect& rect = option.rect;
        QVariant var = index.data(Qt::UserRole + 1);
        MessageItem itemData = var.value<MessageItem>();

        // 置顶标识
        if (mChatObject->getIsTop()) {
            QPainterPath path;
            path.moveTo(rect.topRight());
            path.lineTo(rect.topRight() - QPointF(5, 0));
            path.lineTo(rect.topRight() - QPointF(0, 5));
            painter->setBrush(QColor("4D7CFE"));
            painter->drawPath(path);
        }

        // 鼠标移过
        //if (option.state.testFlag(QStyle::State_MouseOver)) {
        //    painter->setBrush(QColor("#F5F6FA"));
        //    painter->drawRect(rect);
        //}

        // 选中
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setBrush(QColor("#4D7CFE"));
            painter->drawRect(0, 0, 4, rect.height());
        }

        // 绘制头像
        QRect avtRect;
        avtRect.setWidth(AVATAR_SIZE);
        avtRect.setHeight(AVATAR_SIZE);
        avtRect.setX(ESpacing::Narrow);
        avtRect.setY((rect.height() - AVATAR_SIZE) / 2);
        Avatar::DrawAvatar(painter, avtRect, mChatObject->getAvatar(), mChatObject->getOnlineState());

        const int ch = avtRect.center().y(); // 矩形中心线高度
        QFont f("Microsoft Yahei", 10); // 字体
        const int timeWidth = getFontPixelWidth(f, mTime); //时间字体的宽度

        // 绘制时间
        QRect timeRect(rect.width() - ESpacing::Std - timeWidth, ESpacing::Std, timeWidth, ch - ESpacing::Std);
        painter->setFont(f);
        painter->setPen(QColor("#A7ADBD"));
        painter->drawText(timeRect, Qt::AlignHCenter | Qt::AlignBottom, mTime);

        // 绘制消息数量
        QRect mcRect(rect.width(), ch, 0, 0);
        if (!mReadFlag) {
            const QString msgCountStr = QString::number(mUnreadMsgCount);

            mcRect.setTop(ch + ESpacing::Tiny);
            mcRect.setRight(rect.width() - ESpacing::Narrow);
            mcRect.setHeight(getFontPixelHeight(f));
            mcRect.setWidth(getFontPixelWidth(f, msgCountStr) + ESpacing::Narrow);

            painter->setPen(QColor("#FFFFFF"));
            painter->setBrush(QColor("#4D7CFE"));
            painter->setFont(f);
            painter->drawText(mcRect, Qt::AlignCenter, msgCountStr);
            painter->drawRoundedRect(mcRect, int((float(mcRect.width()) / mcRect.height()) * 100), 100);
        }

        // 绘制消息概要
        QRect msgRect;
        msgRect.setLeft(avtRect.right() + ESpacing::Narrow);
        msgRect.setTop(ch + ESpacing::Tiny);
        msgRect.setRight(mcRect.left() + ESpacing::Std);
        msgRect.setBottom(rect.height() - ESpacing::Std);
        painter->setPen(QColor("#A7ADBD"));
        painter->drawText(msgRect, Qt::AlignLeft | Qt::AlignTop, getElidedText(mMessage, f, msgRect.width()));

        // 绘制名称
        QRect nameRect;
        nameRect.setLeft(msgRect.left());
        nameRect.setRight(timeRect.left() + ESpacing::Std);
        nameRect.setTop(ESpacing::Std);
        nameRect.setBottom(ch);
        f.setPointSize(16);
        f.setBold(true);
        painter->setFont(f);
        painter->setPen(QColor("#2D3135"));
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignBottom, mChatObject->getNickName());

        painter->restore();
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
