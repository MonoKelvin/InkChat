#include "ChatItem.h"

#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>

ChatItem::ChatItem(QWidget *parent)
    : QWidget(parent)
    , mUserType(EUserType::Other)
    , mSendState(EChatSendState::Sending)
{
    mLoading = new QLabel(this);
    mLoading->setMovie(new QMovie(this));
    mLoading->resize(16, 16);
    mLoading->setAttribute(Qt::WA_TranslucentBackground, true);
    mLoading->setAutoFillBackground(false);
}

ChatItem::ChatItem(QSharedPointer<User> user, const QString &msg, const QString &time, QWidget *parent)
    : QWidget(parent)
    , mUserType(EUserType::Other)
    , mSendState(EChatSendState::Sending)
{

}

void ChatItem::init()
{

}

const QString ChatItem::getMessage() const
{
    return mMessage->text();
}

const QString ChatItem::getTime() const
{
    return mTime->text();
}
