#include "ChatListView.h"

#include "../Configuation.h"
#include "Avatar.h"
#include "ChatItem.h"
#include "ChatInputBox.h"

#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QListWidgetItem>

ChatListView::ChatListView(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListView::Adjust);
    setSpacing(ESpacing::Narrow);

    mHeaderTitle = new QLabel("Tony Stack", this);
    mHeaderTitle->setAlignment(Qt::AlignCenter);
    mHeaderTitle->setObjectName(QStringLiteral("header"));

    mBtnTool = new QPushButton(this);
    mBtnTool->setFixedSize(16, 16);
    mBtnTool->setObjectName(QStringLiteral("toolBtn"));

    QSharedPointer<User> u = QSharedPointer<User>(new User);
    u->NickName = "Peter Park";
    u->Avatar = Avatar::GetDefaultPixmap();

    /////////////////////////////////////////////////////////
    /// 测试
    /////////////////////////////////////////////////////////
    srand(unsigned(time(nullptr)));
    for (int j = 0; j< 20; j++) {
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
    }
    /////////////////////////////////////////////////////////

    // 每次滚动时，更新items
    connect(verticalScrollBar(), &QScrollBar::valueChanged, [=] {
        updateViewport();
    });

    resizeEvent(nullptr);
}

ChatListView::ChatListView(QSharedPointer<User> user, QWidget *parent)
{

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

        updateViewport();
    }
    // TODO: else { throw error }
}

void ChatListView::clearChats()
{
    const int c = count();

    QListWidgetItem *item;
    for (int index = 0; index < c; index++)
    {
        item = takeItem(0);
        if (item) {
            setItemWidget(item, nullptr);
            delete item;
            item = nullptr;
        }
    }
}

void ChatListView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)

    // 更新组件
    mHeaderTitle->setGeometry(0, 0, width(), mHeaderTitle->height());
    mBtnTool->move(width() - mBtnTool->width() - ESpacing::Std, (mHeaderTitle->height() - mBtnTool->height()) / 2);

    updateViewport();
}

void ChatListView::updateViewport()
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
