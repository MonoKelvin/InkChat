#ifndef FILECHATITEM_H
#define FILECHATITEM_H

#include <ChatItem.h>

class FileChatItem : public ChatItem
{
public:
    FileChatItem();

    void paintContent(QPainter* painter, const QRect& availableRect) override;
    void updateContentSize(const QStyleOptionViewItem&) override;

    inline void setFileChatItemData(const STcpFileData& fileData)
    {
        auto data = mData.value<SChatItemData>();
        data.Message.setValue(fileData);
        mData.setValue(data);
    }

    inline const STcpFileData getFileChatItemData(void)
    {
        return getChatItemData().Message.value<STcpFileData>();
    }

private:
    /** 占百分之几 */
    short mPercentage;
};

#endif // FILECHATITEM_H
