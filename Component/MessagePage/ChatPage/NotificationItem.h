﻿#ifndef NOTIFICATIONITEM_H
#define NOTIFICATIONITEM_H

#include <AbstractChatListItem.h>

class NotificationItem : public AbstractChatListItem
{
    Q_OBJECT

    CHAT_ITEM_OBJECT(NotificationItem, Notification)
public:
    explicit NotificationItem(QObject* parent = nullptr)
        : AbstractChatListItem(parent)
    {
    }

    explicit NotificationItem(const QString& notice, QObject* parent = nullptr)
        : AbstractChatListItem(parent)
    {
        mData = notice;
    }

    void paintContent(QPainter* painter, const QRect& rect) override;
    void updateContentSize(const QStyleOptionViewItem& option) override;

    inline bool selfPaint() override { return true; }
};

#endif // NOTIFICATIONITEM_H
