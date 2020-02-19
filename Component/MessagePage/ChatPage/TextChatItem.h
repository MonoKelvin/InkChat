﻿#ifndef TEXTCHATITEM_H
#define TEXTCHATITEM_H

#include <IChatItem.h>

/**
 * @brief 普通文本聊天项，只能发送文字、字符、表情等简单文本内容。
 * @note 如果要发送富文本（支持字体加粗、斜体、下划线、链接等html文本内容），可以使用RichTextChatItem
 * @see RichTextChatItem
 */
class TextChatItem : public IChatItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText CONSTANT)
public:
    TextChatItem(QObject* parent = nullptr);

    inline const QString getText() const { return mText; }
    inline void setText(const QString& text) { mText = text; }

    void receiveData(QByteArray& data) override;
    void sendData(QByteArray& data) override;

    void package(QVariantMap& data) override;
    void unpackage(QVariantMap& data) override;

    Q_INVOKABLE inline const QString qmlFile() override
    {
        return QStringLiteral("TextChatItem.qml");
    }

private:
    /** 聊天的文本内容 */
    QString mText;
};

#endif // TEXTCHATITEM_H