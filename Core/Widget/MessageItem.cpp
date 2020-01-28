#include "MessageItem.h"

#include "../Configuation.h"
#include "../Utility/Utility.h"
#include "MessageList.h"
#include "Avatar.h"
#include "Badge.h"

MessageItem::MessageItem(unsigned int id, MessageList *list)
    : QWidget(list)
    , mBadge(nullptr)
    , mMessageNumber(0)
    , mID(id)
    , mIsRead(false)
{
    mAvatar = new Avatar(this);
    mName = new QLabel(this);
    mMessage = new QLabel(this);

    mName->setObjectName(QStringLiteral("name"));
    mMessage->setObjectName(QStringLiteral("message"));

    setParent(list);
    setFixedHeight(mAvatar->height() + ESpacing::Narrow + ESpacing::Narrow);

    // 移动控件到相应的位置
    mAvatar->move(ESpacing::Narrow, ESpacing::Narrow);
    mName->move(mAvatar->geometry().right() + ESpacing::Narrow, ESpacing::Narrow);
    mMessage->move(mName->x(), mName->geometry().bottom());

    list->addMessage(this);
}

void MessageItem::setName(const QString &name)
{
    mName->setText(name);
    updateContents();
}

void MessageItem::setMessage(const QString &message)
{
    mMessage->setText(message);
    updateContents();
}

void MessageItem::setUnreadMessageNumber(unsigned int number)
{
    mMessageNumber = number;

    setBadge(QString::number(mMessageNumber));
    updateContents();
}

void MessageItem::readMessage()
{
    //    mMessageNumber = 0;
    mIsRead = true;

    const auto bs = findChildren<Badge *>();
    if (bs.isEmpty())
    {
        return;
    }

    qDeleteAll(bs);
    emit onRead(mID);
}

void MessageItem::unreadMessage()
{
    if (mMessageNumber > 0)
    {
        mIsRead = false;

        setBadge(QString::number(mMessageNumber));
    }
}

void MessageItem::updateContents()
{
    // 设置控件的宽度
    int dw = width() - ESpacing::Narrow;
    if (mBadge) {
        mBadge->move(width() - mBadge->width() - ESpacing::Narrow, (height() - mBadge->height()) / 2);
        dw = mBadge->geometry().x() - ESpacing::Tiny;
    }
    dw -= mName->x();

    getElidedText(mName, dw);
    getElidedText(mMessage, dw);
}

void MessageItem::setBadge(const QString &text)
{
    bool ok = false;
    const int num = text.toInt(&ok);

    if (!mBadge) {
        mBadge = new Badge(this);
    }

    if (ok) {
        if (num > 99) {
            mBadge->setText(QStringLiteral("99+"));
        } else if (num > 0) {
            mBadge->setNum(num);
        } else {
            mBadge->deleteLater();
            return;
        }
    } else {
        mBadge->setText(text);
    }

    const int w = mBadge->fontMetrics().horizontalAdvance(mBadge->text()) + ESpacing::Std;
    const int h = mBadge->fontMetrics().lineSpacing();
    mBadge->resize(w, h);
}
