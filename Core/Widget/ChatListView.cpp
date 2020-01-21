#include "ChatListView.h"

#include "../Configuation.h"
#include "Avatar.h"
#include "ChatItem.h"

#include <QPushButton>
#include <QListWidgetItem>

ChatListView::ChatListView(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListWidget::Adjust);
    setSpacing(ESpacing::Std);


    QSharedPointer<User> u = QSharedPointer<User>(new User);
    u->setNickName("Tony Stack");
    u->setAvatar(Avatar::GetDefaultPixmap());

    /// 测试
    QPushButton *btn = new QPushButton("添加对话", this->parentWidget());
    srand(unsigned(time(nullptr)));
    connect(btn, &QPushButton::clicked, [ = ]
    {
        QString s("a");
        int i = 0;
        while (i++ < rand() % 100)
        {
            s += "a";
            if (rand() % 100 < 20 == 0)
            {
                s += " ";
            }
        }
        addChatItem(new ChatItem(u, s));
    });
}

void ChatListView::addChatItem(ChatItem *chat)
{
    if (chat)
    {
        chat->setParent(this);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(chat->size());
        addItem(item);
        setItemWidget(item, chat);
    }
}

void ChatListView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)

    const int items = count();
    for (int index = 0; index < items; index++)
    {
        QListWidgetItem *i = item(index);
        ChatItem *ci = static_cast<ChatItem *>(itemWidget(i));

        ci->updateHeight();
        ci->setFixedWidth(width());
        i->setSizeHint(ci->size());
//        setItemWidget(i, ci);
    }
}
