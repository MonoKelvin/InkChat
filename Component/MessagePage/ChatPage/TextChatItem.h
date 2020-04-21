#ifndef TEXTCHATITEM_H
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

    CHATITEM_OBJECT(TextChatItem, Text)
public:
    void praseData(const QVariant& data) override;
    const QVariant getData(void) override;

    void paintContent(QPainter* painter, const QRect& rect) override;
    void updateContentSize(const QStyleOptionViewItem& option) override;

private:
    /** 聊天的文本内容 */
    QString mText;
};

#endif // TEXTCHATITEM_H
