#include "MessageItem.h"

#include "../Configuation.h"
#include "../Utility/Utility.h"
#include "Avatar.h"
#include "Badge.h"

#include <QBoxLayout>
#include <QLabel>

MessageItem::MessageItem(unsigned int id, QWidget *parent)
    : QWidget(parent)
    , mBadge(nullptr)
    , mMessageNumber(0)
    , mID(id)
    , mIsRead(false)
{
    mAvatar = new Avatar(this);
    mName = new QLabel(this);
    mMessage = new QLabel(this);

    mName->setObjectName(QLatin1String("name"));
    mMessage->setObjectName(QLatin1String("message"));

    setFixedHeight(mAvatar->height() + ESpacing::Narrow + ESpacing::Narrow);
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

        layout()->addWidget(new Badge(int(mMessageNumber), this));
    }
}

void MessageItem::updateContents()
{
    // 1. 移动控件到相应的位置
    mAvatar->move(ESpacing::Narrow, ESpacing::Narrow);
    mName->move(mAvatar->geometry().right() + ESpacing::Narrow, ESpacing::Narrow);
    mMessage->move(mName->x(), mName->geometry().bottom() + ESpacing::Tiny);

    // 2.设置控件的宽度
    int dw = width() - ESpacing::Narrow;
    if (mBadge) {
        mBadge->move(width() - mBadge->width() - ESpacing::Narrow, (height() - mBadge->height()) / 2);
        dw = mBadge->geometry().x();
    }
    dw -= mName->x();

    mName->setFixedWidth(dw);
    mName->setText(getElidedText(mName->text(), mName->font(), dw));

    mMessage->setFixedWidth(dw);
    mMessage->setText(getElidedText(mMessage->text(), mMessage->font(), dw));
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
            mBadge->setText(QLatin1String("99+"));
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
