#include "TextChatItem.h"

#include <QVariantMap>

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
