#include "MessageList.h"

#include "../Utility/Utility.h"
#include "MessageItem.h"
#include "Avatar.h"

#include <QListWidgetItem>

MessageList::MessageList(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListView::Adjust);
    setSpacing(0);

}

void MessageList::addMessage(MessageItem *msg)
{
    if (msg) {
        msg->setFixedWidth(width());
        msg->updateContents();

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(msg->size());

        addItem(item);
        setItemWidget(item, msg);
    }
}
