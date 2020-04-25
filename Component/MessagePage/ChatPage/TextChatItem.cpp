#include "TextChatItem.h"

#include <AppTheme.h>
#include <User.h>
#include <Utility.h>

TextChatItem::TextChatItem(QObject* parent)
    : ChatItem(parent)
{
}

void TextChatItem::paintContent(QPainter* painter, const QRect& rect)
{
    const int& d = ESize::Std;
    const int& sd = ESize::Narrow;

    // 背景
    QRect bg;
    bg.setTopLeft(rect.topLeft());
    bg.setWidth(mContentSize.width());
    bg.setHeight(mContentSize.height() - ESize::Wide);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制气泡背景
    if (getChatItemData().Uuid == User::Instance()->getUuid()) {
        bg.moveRight(rect.right());

        // 背景
        painter->setPen(XTheme.RightBubbleStyle.BorderColor);
        painter->setBrush(XTheme.RightBubbleStyle.BubbleColor);
        drawRoundRect(painter, bg, sd, 0, sd, sd);
        painter->setRenderHint(QPainter::Antialiasing, false);

        // 时间
        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->setPen(XTheme.SubTextColor);
        painter->setFont(XTheme.SmallFont);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignLeft, getMessageTime());

        // 内容
        painter->setPen(XTheme.RightBubbleStyle.TextColor);
    } else {
        painter->setPen(XTheme.LeftBubbleStyle.BorderColor);
        painter->setBrush(XTheme.LeftBubbleStyle.BubbleColor);
        drawRoundRect(painter, bg, 0, sd, sd, sd);
        painter->setRenderHint(QPainter::Antialiasing, false);

        const QRect timeRect(bg.x(), bg.bottom() + ESize::Tiny, qMax(50, mContentSize.width()), ESize::Std);
        painter->setPen(XTheme.SubTextColor);
        painter->setFont(XTheme.SmallFont);
        painter->drawText(timeRect, Qt::AlignTop | Qt::AlignRight, getMessageTime());

        painter->setPen(XTheme.LeftBubbleStyle.TextColor);
    }

    // 绘制文本
    painter->setFont(XTheme.StdFont);
    painter->drawText(bg.adjusted(d, sd, -d, -sd), Qt::TextWrapAnywhere, getChatItemData().Message.toString());
    painter->restore();
}

void TextChatItem::updateContentSize(const QStyleOptionViewItem& option)
{
    const QRect limit(0, 0, int(0.7f * option.rect.width()), 0);
    mContentSize = option.fontMetrics.boundingRect(limit, Qt::TextWrapAnywhere, getChatItemData().Message.toString()).size();
    mContentSize.rwidth() += ESize::Std + ESize::Std;
    mContentSize.rheight() += ESize::Narrow + ESize::Narrow + ESize::Wide;
}
