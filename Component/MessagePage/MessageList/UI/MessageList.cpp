#include "MessageList.h"

#include "MessageItem.h"
//#include "Avatar.h"

//#include <QScrollBar>
#include <QListWidgetItem>

MessageList::MessageList(QWidget *parent)
    : SearchableList(parent)
{
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
