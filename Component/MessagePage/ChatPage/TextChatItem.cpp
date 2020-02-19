#include "TextChatItem.h"

#include <QDataStream>

TextChatItem::TextChatItem(QObject* parent)
    : IChatItem(parent)
{
    mContentType = Text;
}

void TextChatItem::receiveData(QByteArray& data)
{
    IChatItem::receiveData(data);

    QDataStream dataStream(&data, QIODevice::ReadOnly);

    dataStream >> mText;
}

void TextChatItem::sendData(QByteArray& data)
{
    IChatItem::sendData(data);

    QDataStream dataStream(&data, QIODevice::WriteOnly);

    dataStream << mText;
}

void TextChatItem::package(QVariantMap& data)
{
}

void TextChatItem::unpackage(QVariantMap& data)
{
}
