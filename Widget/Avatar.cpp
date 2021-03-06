﻿#include "Avatar.h"

#include <AppTheme.h>
#include <Utility.h>

Avatar::Avatar(QWidget* parent)
    : QWidget(parent)
    , mOnlineState(IChatObject::Offline)
{
    setAutoFillBackground(true);
    setFixedSize(XTheme.AvatarSize, XTheme.AvatarSize);
    setCursor(Qt::PointingHandCursor);
}

Avatar::Avatar(const QPixmap& pixmap, QWidget* parent)
    : QWidget(parent)
    , mOnlineState(IChatObject::Offline)
{
    setAutoFillBackground(true);
    setAvatar(pixmap);
    setFixedSize(XTheme.AvatarSize, XTheme.AvatarSize);
    setCursor(Qt::PointingHandCursor);
}

Avatar::~Avatar()
{
}

void Avatar::DrawAvatar(QPainter* painter, const QRect& rect, const QPixmap& pixmap, const QString& defalultText, IChatObject::EOnlineState state)
{
    painter->save();

    QPainterPath path;
    path.addEllipse(rect);

    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush("#E5ECFF"));
    painter->setClipPath(path);

    // 当没有图片时绘制背景色
    if (pixmap.isNull()) {
        painter->drawRect(rect);

        const auto& trimmedStr = defalultText.trimmed();
        if (!trimmedStr.isEmpty()) {
            painter->setFont(XTheme.TitleFont);
            painter->setPen(XTheme.PrimayColor1);
            painter->drawText(rect, Qt::AlignCenter, trimmedStr.at(0));
        }
    } else {
        painter->drawPixmap(rect, pixmap);
    }

    if (state != IChatObject::NoneState) {
        qreal d = ESize::Narrow;
        qreal offset = 0.8535 * rect.width() - 0.5 * d;
        QRectF rt(offset, offset, d, d);

        painter->setPen(Qt::NoPen);

        switch (state)
        {
        case IChatObject::Online:
            painter->setBrush(QBrush(XTheme.PrimayColor1));
            break;
        case IChatObject::Offline:
            painter->setBrush(QBrush(XTheme.SubTextColor));
            break;
        case IChatObject::Busy:
            painter->setBrush(QBrush(XTheme.PrimayColor2));
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
