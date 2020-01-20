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
    init();
}

MessageItem::MessageItem(unsigned int id,
                         const QPixmap &avatar,
                         const QString &name,
                         const QString &message,
                         QWidget *parent)
    : QWidget(parent)
    , mName(name)
    , mMessage(message)
    , mMessageNumber(0)
    , mID(id)
    , mIsRead(false)
{
    mAvatar = new Avatar(avatar, this);
    init();
}

void MessageItem::init()
{
    QLabel *name = new QLabel(mName, this);
    QLabel *message = new QLabel(mMessage, this);

    // 主布局
    QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    hLayout->setSpacing(ESpacing::Std);
    hLayout->setMargin(0);

    // 名字和消息布局
    QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    vLayout->setSpacing(ESpacing::Narrow);
    vLayout->setMargin(0);

    hLayout->addWidget(mAvatar);

    vLayout->addStretch(1);
    name->setObjectName(QLatin1String("name"));
    vLayout->addWidget(name, 1);
    message->setObjectName(QLatin1String("message"));
    vLayout->addWidget(message, 1);
    vLayout->addStretch(1);

    hLayout->addLayout(vLayout);
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
