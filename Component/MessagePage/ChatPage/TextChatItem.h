#ifndef TEXTCHATITEM_H
#define TEXTCHATITEM_H

#include <ChatItem.h>

/**
 * @brief 普通文本聊天项，只能发送文字、字符、表情等简单文本内容。
 * @note 如果要发送富文本（支持字体加粗、斜体、下划线、链接等html文本内容），可以使用RichTextChatItem
 * @see RichTextChatItem
 */
class TextChatItem : public ChatItem
{
    Q_OBJECT

    CHAT_ITEM_OBJECT(TextChatItem, Text)
public:
    explicit TextChatItem(QObject* parent = nullptr)
        : ChatItem(parent)
    {
    }

    void paintContent(QPainter* painter, const QRect& rect) override;
    void updateContentSize(const QStyleOptionViewItem& option) override;
};

#endif // TEXTCHATITEM_H
