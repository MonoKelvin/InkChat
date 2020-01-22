#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QListWidget>

class MessageItem;

class MessageList : public QListWidget
{
    Q_OBJECT
public:
    explicit MessageList(QWidget *parent = nullptr);

    void addMessage(MessageItem *msg);

};

#endif // MESSAGELIST_H
