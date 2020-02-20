#include "TextChatItem.h"

#include <QVariantMap>

TextChatItem::TextChatItem(QObject* parent)
    : IChatItem(parent)
{
}

void TextChatItem::package(QVariantMap& data)
{
    data.insert(QStringLiteral("data"), mText);

    return IChatItem::package(data);
}

void TextChatItem::unpackage(QVariantMap& data)
{
    mText = data.value(QStringLiteral("data")).toString();

    return IChatItem::unpackage(data);
}
