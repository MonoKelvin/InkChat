#include "Avatar.h"

#include "../Configuation.h"

#include <QPainter>

Avatar::Avatar(QWidget *parent)
    : QWidget(parent)
    , mOnlineState(User::Offline)
{
    setDefaultAvatar();
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

Avatar::Avatar(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , mOnlineState(User::Offline)
{
    setAvatar(pixmap);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

Avatar::~Avatar()
{

}

void Avatar::DrawAvatar(QPainter *painter, const QRect &rect, const QPixmap &pixmap)
{
    painter->save();

    QPainterPath path;
    path.addEllipse(rect);

    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setClipPath(path);
    painter->drawPixmap(rect, pixmap);

    painter->restore();
}

void Avatar::mousePressEvent(QMouseEvent *event)
{
    emit onClick();
    return QWidget::mousePressEvent(event);
}

void Avatar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    if (!mAvatar.isNull())
    {
        Avatar::DrawAvatar(&painter, rect(), mAvatar);
    }

    if (mOnlineState != User::None)
    {
        qreal d = 10.0;
        qreal offset = 0.8535 * width() - 0.5 * d;
        QRectF rt(offset, offset, d, d);

        painter.setPen(Qt::NoPen);
        painter.setRenderHints(QPainter::Antialiasing, true);

        switch (mOnlineState)
        {
        case User::Online:
            painter.setBrush(QBrush(ONLINE_STATE_COLOR));
            break;
        case User::Offline:
            painter.setBrush(QBrush(OFFLINE_STATE_COLOR));
            break;
        case User::Busy:
            painter.setBrush(QBrush(BUSY_STATE_COLOR));
            break;
        default:
            break;
        }

        painter.drawEllipse(rt);
    }

}
