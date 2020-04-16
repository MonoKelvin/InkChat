#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include "SearchableList.h"

class MessageItem;
class QPushButton;

class MessageList : public SearchableList
{
    Q_OBJECT
public:
    explicit MessageList(QWidget *parent = nullptr);

    void addMessage(MessageItem *msg);
};

#endif // MESSAGELIST_H
