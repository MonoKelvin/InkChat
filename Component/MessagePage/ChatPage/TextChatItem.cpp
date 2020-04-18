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

const QRect TextChatItem::paintContent(QPainter* painter, const QRect& availableRect)
{
    painter->save();

    QFont f(QStringLiteral("Microsoft Yahei"), 10);
    painter->setFont(f);
    painter->setPen(Qt::NoPen);

    const int fw = getFontPixelWidth(f, mText); // 文字的总宽度
    const int msgRW = qMin(int(0.7f * availableRect.width()), fw); //  文字字体真实总宽度
    const int msgRH = painter->fontMetrics().lineSpacing() * qMax(Ceil(fw / float(msgRW)), 1); // 消息内容真实总行高

    QRect rect(availableRect.right() - msgRW - ESpacing::Wide, 0, msgRW + ESpacing::Wide, msgRH + ESpacing::Wide);
    if (mChatObject->getRoleType() == IChatObject::Me) {
        painter->setBrush(QBrush("#F5F6FA"));
        drawRoundRect(painter, rect, 10, 0);

        painter->setPen(QPen("#2D3135"));
    } else {
        rect.setLeft(availableRect.left());
        painter->setBrush(QBrush("#4D7CFE"));
        drawRoundRect(painter, rect, 0, 10);

        painter->setPen(QPen(Qt::white));
    }

    QTextOption tp;
    tp.setWrapMode(QTextOption::WrapAnywhere);
    painter->drawText(rect.adjusted(ESpacing::Narrow, ESpacing::Narrow, ESpacing::Narrow, ESpacing::Narrow), mText, tp);

    painter->restore();
    return rect;
}
