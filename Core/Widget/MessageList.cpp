#include "MessageList.h"

#include "MessageItem.h"
#include "Avatar.h"

#include <QListWidgetItem>
#include <QStandardItemModel>

MessageList::MessageList(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListView::Adjust);
    setSpacing(0);

    addMessage(new MessageItem(1, QPixmap(), "测试", ""));
    addMessage(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词用英语怎么说"));
    addMessage(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词库精选标记为未读用英语怎么说"));
    addMessage(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词库精选沪江词库精选标记为未读用英语怎么说标记为未读用英语怎么说"));
}

void MessageList::addMessage(MessageItem *msg)
{
    if (msg) {
        msg->setParent(this);
        msg->setFixedWidth(width());

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(msg->size());

        addItem(item);
        setItemWidget(item, msg);
    }
}
