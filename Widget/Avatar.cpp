#include "Avatar.h"

#include <Configuation.h>
#include <Utility.h>

#include <QPainter>

Avatar::Avatar(QWidget *parent)
    : QWidget(parent)
    , mOnlineState(User::Offline)
{
    setAutoFillBackground(true);
    setFixedSize(AVATAR_SIZE, AVATAR_SIZE);
    setCursor(Qt::PointingHandCursor);
}

Avatar::Avatar(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , mOnlineState(User::Offline)
{
    setAutoFillBackground(true);
    setAvatar(pixmap);
    setFixedSize(AVATAR_SIZE, AVATAR_SIZE);
    setCursor(Qt::PointingHandCursor);
}

Avatar::~Avatar()
{

}

void Avatar::DrawAvatar(QPainter *painter, const QRect &rect, const QPixmap &pixmap, User::EOnlineState state)
{
    painter->save();

    QPainterPath path;
    path.addEllipse(rect);

    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setClipPath(path);
    painter->drawPixmap(rect, pixmap);

    if (state != User::NoneState)
    {
        qreal d = ONLINE_STATE_DOT_SIZE;
        qreal offset = 0.8535 * rect.width() - 0.5 * d;
        QRectF rt(offset, offset, d, d);

        painter->setPen(Qt::NoPen);

        switch (state)
        {
        case User::Online:
            painter->setBrush(QBrush(ONLINE_STATE_COLOR));
            break;
        case User::Offline:
            painter->setBrush(QBrush(OFFLINE_STATE_COLOR));
            break;
        case User::Busy:
            painter->setBrush(QBrush(BUSY_STATE_COLOR));
            break;
        default:
            break;
        }

        painter->drawEllipse(rt);
    }

    painter->restore();
}

void Avatar::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    emit onClick();
//    return QWidget::mousePressEvent(event);
}

void Avatar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (mAvatar.isNull()) {
        USE_STYLE_SHEET
    } else {
        Avatar::DrawAvatar(&painter, rect(), mAvatar);
    }

    return QWidget::paintEvent(event);
}
