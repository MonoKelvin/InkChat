#include "ChatItem.h"

#include "Avatar.h"
#include "../Utility/Utility.h"
#include "../Configuation.h"

#include <QBoxLayout>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>
#include <QFontMetrics>

ChatItem::ChatItem(QSharedPointer<User> user, const QString &msg, ESender sender, QWidget *parent)
    : QWidget(parent)
    , mSender(sender)
    , mSendState(ESendState::Sending)
{
    setAttribute(Qt::WA_TranslucentBackground);

    // 1.主水平布局
    QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    hLayout->setSpacing(ESpacing::Std);
    hLayout->setMargin(0);

    // 2.头像布局
    QBoxLayout *avLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    avLayout->setSpacing(0);
    avLayout->setMargin(0);

    // 头像
    mAvatar = new Avatar(user->getAvatar(), this);
    mAvatar->setObjectName(QLatin1String("mAvatar"));
    avLayout->addWidget(mAvatar);
    avLayout->addStretch(1);

    // 3.名称、消息、时间等布局
    QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    vLayout->setSpacing(ESpacing::Std);

    // 名称（只有群聊才有内容，否则只是占位）
    QLabel *lbName = new QLabel(this);
    lbName->setObjectName(QLatin1String("name"));
    lbName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    lbName->setFixedHeight(mAvatar->height() / 2 - ESpacing::Std);
    vLayout->addWidget(lbName);

    // 消息
    mMessage = new QLabel(msg, this);
    mMessage->setMaximumWidth(100);
    mMessage->setWordWrap(true);
    mMessage->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMessage->setObjectName(QLatin1String("message_ta"));
    mMessage->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
    vLayout->addWidget(mMessage);

    // 发送时间
    mTime = new QLabel(QDateTime::currentDateTime().toString("hh:mm:ss"), this);
    mTime->setObjectName(QLatin1String("time"));
    mTime->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mTime->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
    vLayout->addWidget(mTime);

    // 正在发送标志
    mSendStateLabel = new QLabel(this);
    mSendStateLabel->resize(ESpacing::Std, ESpacing::Std);
    mSendStateLabel->setScaledContents(true);
    mSendStateLabel->setObjectName(QLatin1String("sendState_sending"));
    mSendStateLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    mSendStateLabel->setAutoFillBackground(false);
    // 设置加载gif动画或图片
#if QT_CONFIG(movie)
    QMovie *movie = new QMovie(":/App/Icons/loading_15x.gif");
    mSendStateLabel->setMovie(movie);
    movie->start();
#elif
    mSendStateLabel->setPixmap(QPixmap(":/App/Icons/loading_200x.png"));
#endif
    mSendStateLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    switch (mSender)
    {
    case ESender::Me:
        hLayout->addStretch(1);
        hLayout->addWidget(mSendStateLabel);
        hLayout->addLayout(vLayout);
        hLayout->addLayout(avLayout);
        break;
    default:
        hLayout->addLayout(avLayout);
        hLayout->addLayout(vLayout);
        hLayout->addWidget(mSendStateLabel);
        hLayout->addStretch(1);
        break;
    }

    mMessage->adjustSize();
    updateHeight();
}

void ChatItem::setMessage(const QString &text)
{
    mMessage->setText(text);
    mMessage->adjustSize();

    updateHeight();
}

void ChatItem::init()
{
}

void ChatItem::updateHeight()
{
    const int len = mMessage->width();
    const auto f = mMessage->fontMetrics();
    const int flen = f.horizontalAdvance(mMessage->text());

    // 如果文字内容的宽度大于label的宽度就增加label的高度
    if (flen > len)
    {
        // 内容行数
        const int lines = qMax(Ceil(flen / float(len)), 1);

        mMessage->setFixedHeight(f.lineSpacing() * lines);
    }

    adjustSize();
}

const QString ChatItem::getMessage() const
{
    return mMessage->text();
}

const QString ChatItem::getTime() const
{
    return mTime->text();
}
