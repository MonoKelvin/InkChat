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
    : IChatWidget(sender, parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    // 头像
    mAvatar = new Avatar(user->getAvatar(), this);
    mAvatar->setOnlineState(User::NoneState);
    mAvatar->setObjectName(QLatin1String("mAvatar"));

    // 名称（只有群聊才有内容，否则只是占位）
    mName = new QLabel(this);
    mName->setObjectName(QLatin1String("name"));
    mName->setFixedHeight(mAvatar->height() / 2);

    // 消息
    mMessage = new QLabel(msg, this);
    mMessage->setWordWrap(true);
    mMessage->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // 发送时间
    mTime = new QLabel(QDateTime::currentDateTime().toString("hh:mm:ss"), this);
    mTime->setObjectName(QLatin1String("time"));
    mTime->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

    // 正在发送标志
    mSendStateLabel = new QLabel(this);
    mSendStateLabel->setFixedSize(ESpacing::Std, ESpacing::Std);
    mSendStateLabel->setScaledContents(true);
    mSendStateLabel->setObjectName(QLatin1String("sendState_sending"));
    mSendStateLabel->setAttribute(Qt::WA_TranslucentBackground);
    mSendStateLabel->setAutoFillBackground(false);
    // 设置加载gif动画或图片
#if QT_CONFIG(movie)
    QMovie *movie = new QMovie(":/App/Icons/loading_15x.gif");
    mSendStateLabel->setMovie(movie);
    movie->start();
#elif
    mSendStateLabel->setPixmap(QPixmap(":/App/Icons/loading_200x.png"));
#endif

    setSender(sender);
}

void ChatItem::setMessage(const QString &text)
{
    mMessage->setText(text);

    updateContents();
}

void ChatItem::setSender(ChatItem::ESender sender)
{
    mSender = sender;

    switch (mSender)
    {
    case ESender::Me:
        mMessage->setObjectName(QLatin1String("message_me"));
        break;
    case ESender::Ta:
        mMessage->setObjectName(QLatin1String("message_ta"));
        break;
    case ESender::Other:
        mMessage->setObjectName(QLatin1String("message_other"));
        break;
    case ESender::System:
        mMessage->setObjectName(QLatin1String("message_system"));
        break;
    }

    updateContents();
}

void ChatItem::updateContents()
{
    if (mIsUpdatedContents)
    {
        return;
    }

    mMessage->adjustSize();

    const int sp = ESpacing::Std; // 标准间隔

    // 计算消息气泡的宽高
    {
        const auto msgF = mMessage->fontMetrics(); // 消息字体
        const int msgFW = msgF.horizontalAdvance(mMessage->text()); // 消息字体总宽度
        int msgRW = qMin(int(0.7f * width()), msgFW); // 消息字体真实总宽度
        const int msgRH = msgF.lineSpacing() * qMax(Ceil(msgFW / float(msgRW)), 1); // 消息内容真实总行高

        mMessage->setMinimumSize(msgRW, msgRH);
    }

    if (ESender::Me == mSender)
    {
        // 1.确定头像位置
        mAvatar->move(width() - mAvatar->width(), 0);

        // 2.确定聊天内容位置
        mMessage->move(mAvatar->x() - mMessage->width() - sp, mAvatar->rect().center().y());

        // 3.确定发送状态位置
        mSendStateLabel->move(mMessage->x() - mSendStateLabel->width() - sp,
                              mMessage->geometry().center().y() - mSendStateLabel->height() / 2);

        mName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
    {
        mAvatar->move(0, 0);
        mMessage->move(mAvatar->geometry().right() + sp, mAvatar->geometry().center().y());
        mName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        mSendStateLabel->move(mMessage->geometry().right() + sp,
                              mMessage->geometry().center().y() - mSendStateLabel->height() / 2);
    }

    // 4.确定名称位置
    mName->move(mMessage->x(), 0);
    mName->setFixedWidth(mMessage->width());

    // 5.确定时间位置
    mTime->move(mMessage->x(), mMessage->geometry().bottom() + ESpacing::Tiny);
    mTime->setFixedWidth(mMessage->width());

    adjustSize();

    mIsUpdatedContents = true;
}

const QString ChatItem::getMessage() const
{
    return mMessage->text();
}

const QString ChatItem::getTime() const
{
    return mTime->text();
}
