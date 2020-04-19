#include "TextChatItem.h"

#include <Configuation.h>
#include <QLabel>
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

    const int& d = ESpacing::Std;
    const QFont f(QStringLiteral("Microsoft Yahei"), 10);
    const QRect limit(rect.x() + d, rect.y() + d, int(0.7f * rect.width()) - 2 * d, 0);
    QRect textRect = painter->fontMetrics().boundingRect(limit, Qt::TextWrapAnywhere, mText);

    // 绘制气泡背景
    painter->setFont(f);
    painter->setPen(Qt::NoPen);
    if (mChatObject->getRoleType() == IChatObject::Me) {
        textRect.moveRight(rect.right() - d);
        mContentArea = textRect.adjusted(-d, -d, d, d);

        painter->setBrush(QBrush("#F5F6FA"));
        drawRoundRect(painter, mContentArea, 10, 0);

        painter->setPen(QPen("#2D3135"));
    } else {
        mContentArea = textRect.adjusted(-d, -d, d, d);

        painter->setBrush(QBrush("#4D7CFE"));
        drawRoundRect(painter, mContentArea, 0, 10);

        painter->setPen(QPen(Qt::white));
    }

    // 绘制文本
    painter->drawText(textRect, Qt::TextWrapAnywhere, mText);
    painter->restore();
}
