#ifndef NOTIFICATIONITEM_H
#define NOTIFICATIONITEM_H

#include <AbstractChatListItem.h>

class NotificationItem : public AbstractChatListItem
{
public:
    NotificationItem(QObject* parent)
        : AbstractChatListItem(parent)
    {
    }

    void paintContent(QPainter* painter, const QRect& availableRect) override;
    void updateContentSize(const QStyleOptionViewItem& option) override;

private:
    QString mContent;
};

#endif // NOTIFICATIONITEM_H
