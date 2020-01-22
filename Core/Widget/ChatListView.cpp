#include "ChatListView.h"

#include "../Configuation.h"
#include "Avatar.h"
#include "ChatItem.h"

#include <QScrollBar>
#include <QPushButton>
#include <QListWidgetItem>

ChatListView::ChatListView(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListWidget::Adjust);
    setSpacing(ESpacing::Narrow);

    QSharedPointer<User> u = QSharedPointer<User>(new User);
    u->setNickName("Tony Stack");
    u->setAvatar(Avatar::GetDefaultPixmap());

    /// 测试
    QPushButton *btn = new QPushButton("添加对话", this->parentWidget());
    srand(unsigned(time(nullptr)));
    connect(btn,
            &QPushButton::clicked,
            [ = ]
    {
        QString s("a");
        int i = 0;
        const int r = rand() % 50 + 1;
        while (i++ < r)
        {
            s += "a";
            if (rand() % 100 < 50)
            {
                s += ' ';
            }
            if (rand() % 100 < 40)
            {
                s += 'I';
            }
            if (rand() % 100 < 30)
            {
                s += "秀";
            }
            if (rand() % 100 < 20)
            {
                s += "纛";
            }
        }
        addChatWidget(new ChatItem(u, s, IChatWidget::ESender::Ta));
        addChatWidget(new ChatItem(u, s));
    });

    // 每次滚动时，更新items
    connect(verticalScrollBar(),
            &QScrollBar::valueChanged,
            [ = ]
    {
        updateViewportItems();
    });
}

void ChatListView::addChatWidget(IChatWidget *chat, bool isScrollToBottom)
{
    if (chat)
    {
        chat->setParent(this);
        QListWidgetItem *item = new QListWidgetItem;

        addItem(item);
        setItemWidget(item, chat);

        if (isScrollToBottom)
        {
            scrollToBottom();
        }

        updateViewportItems();
    }
    // TODO: else { throw error }
}

void ChatListView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)

    updateViewportItems();
}

void ChatListView::updateViewportItems()
{
    // 垂直扫描item并更新它们的尺寸
    for (int scan = 1; scan <= height();)
    {
        const auto i = (itemAt(width() / 2, scan));
        const auto chat = static_cast<IChatWidget *>(itemWidget(i));

        if (chat)
        {
            chat->setFixedWidth(width());
            chat->updateContents();
            i->setSizeHint(chat->size());
        }

        // 每次增加1个间隔，再-1是为了确保一定能命中一个item
        scan += this->spacing() - 1;
    }
}
