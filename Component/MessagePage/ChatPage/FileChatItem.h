#ifndef FILECHATITEM_H
#define FILECHATITEM_H

#include <ChatItem.h>

#include <QFileInfo>

class FileChatItem : public ChatItem
{
    Q_OBJECT

    CHAT_ITEM_OBJECT(FileChatItem, File)

public:
    explicit FileChatItem(QObject* parent = nullptr);

    void paintContent(QPainter* painter, const QRect& rect) override;
    void updateContentSize(const QStyleOptionViewItem& option) override;

    /**
     * @brief 打开文件路径
     * @return 如果路径失效或文件不存在则返回false，否则返回true
     */
    bool openFileDir(void);

    void setChatItemData(const SChatItemData& data) noexcept override;

public:
    float Speed;
    unsigned char Percentage;

private:
    QFileInfo mFileInfo;
};

#endif // FILECHATITEM_H
