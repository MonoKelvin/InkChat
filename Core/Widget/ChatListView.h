#ifndef CHATLISTVIEW_H
#define CHATLISTVIEW_H

#include <QListWidget>

class ChatItem;

class ChatListView : public QListWidget
{
    Q_OBJECT
public:
    ChatListView(QWidget *parent = nullptr);

    void addChatItem(ChatItem *chat);
};

#endif // CHATLISTVIEW_H
