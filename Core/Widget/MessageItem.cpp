#include "MessageItem.h"

#include "../Configuation.h"
#include "Avatar.h"
#include "Badge.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>

MessageItem::MessageItem(unsigned int id, QWidget *parent)
    : QWidget(parent)
    , mMessageNumber(0)
    , mID(id)
    , mIsRead(false)
{
    mAvatar = new Avatar(this);
    mName = new QLabel(this);
    mMessage = new QLabel(this);
    init();
}

MessageItem::MessageItem(unsigned int id,
                         const QPixmap &avatar,
                         const QString &name,
                         const QString &message,
                         QWidget *parent)
    : QWidget(parent)
    , mMessageNumber(0)
    , mID(id)
    , mIsRead(false)
{
    mAvatar = new Avatar(avatar, this);
    mName = new QLabel(name, this);
    mMessage = new QLabel(message, this);
    init();
}

void MessageItem::init()
{
    // 主布局
    QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    hLayout->setSpacing(ESpacing::Std);
    hLayout->setMargin(0);

    hLayout->addWidget(mAvatar);

    // 名字和消息布局
    QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    vLayout->setSpacing(0);
    vLayout->setMargin(0);

    mName->setObjectName(QLatin1String("name"));
    mName->setMinimumHeight(mName->fontMetrics().height());
    mName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vLayout->addWidget(mName);

    mMessage->setObjectName(QLatin1String("message"));
    mMessage->setFixedHeight(mMessage->fontMetrics().height());
    mMessage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vLayout->addWidget(mMessage);

    hLayout->addLayout(vLayout);

    adjustSize();
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
