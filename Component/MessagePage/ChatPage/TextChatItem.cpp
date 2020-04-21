#include "TextChatItem.h"

#include <AppTheme.h>
#include <QVariant>
#include <Utility.h>

TextChatItem::TextChatItem(QObject* parent)
    : IChatItem(parent)
{
}

void TextChatItem::praseData(const QVariant& data)
{
    mText = data.toString();
}

const QVariant TextChatItem::getData()
{
    return mText;
}

void TextChatItem::paintContent(QPainter* painter, const QRect& rect)
{
    painter->save();

    const int& d = ESize::Std;
    const int& sd = ESize::Narrow;

    // 背景
    QRect bg;
    bg.setTopLeft(rect.topLeft());
    bg.setSize(mContentSize);

    // 绘制气泡背景
    painter->setFont(XTheme.StdFont);
    painter->setRenderHint(QPainter::Antialiasing);
    if (mChatObject->getRoleType() == IChatObject::Me) {
        bg.moveRight(rect.right());

        painter->setPen(XTheme.RightBubbleStyle.BorderColor);
        painter->setBrush(XTheme.RightBubbleStyle.BubbleColor);
        drawRoundRect(painter, bg, sd, 0, sd, sd);

        painter->setPen(XTheme.RightBubbleStyle.TextColor);
    } else {
        painter->setPen(XTheme.LeftBubbleStyle.BorderColor);
        painter->setBrush(XTheme.LeftBubbleStyle.BubbleColor);
        drawRoundRect(painter, bg, 0, sd, sd, sd);

        painter->setPen(XTheme.LeftBubbleStyle.TextColor);
    }

    // 绘制文本
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawText(bg.adjusted(d, sd, -d, -sd), Qt::TextWrapAnywhere, mText);
    painter->restore();
}

void TextChatItem::updateContentSize(const QStyleOptionViewItem& option)
{
    const QRect limit(0, 0, int(0.7f * option.rect.width()), 50);
    mContentSize = option.fontMetrics.boundingRect(limit, Qt::TextWrapAnywhere, mText).size();
    mContentSize.rwidth() += ESize::Std + ESize::Std;
    mContentSize.rheight() += ESize::Narrow + ESize::Narrow;
}
