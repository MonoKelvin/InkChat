#include "NotificationItem.h"

#include <AppTheme.h>
#include <Utility.h>

#include <QPainter>

void NotificationItem::paintContent(QPainter* painter, const QRect& rect)
{
    QRect bg;
    bg.setSize(mContentSize);
    bg.moveTop(rect.top());
    bg.moveLeft((rect.width() - bg.width()) / 2);

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(XTheme.TintColor);
    painter->setRenderHint(QPainter::Antialiasing);
    DrawRoundRect(painter, bg, 4, 4, 4, 4);

    painter->setFont(XTheme.StdFont);
    painter->setPen(XTheme.PrimayColor1);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawText(bg.adjusted(ESize::Narrow, ESize::Tiny, -ESize::Narrow, -ESize::Tiny), Qt::AlignCenter, mData.toString());
    painter->restore();
}

void NotificationItem::updateContentSize(const QStyleOptionViewItem& option)
{
    const QRect limit(0, 0, int(0.7f * option.rect.width()), 0);
    mContentSize = option.fontMetrics.boundingRect(limit, Qt::TextWrapAnywhere, mData.toString()).size();
    mContentSize.rwidth() += ESize::Narrow + ESize::Narrow;
    mContentSize.rheight() += ESize::Tiny + ESize::Tiny;
}
