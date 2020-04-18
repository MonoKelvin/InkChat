#include "ChatViewWidget.h"
#include "ui_chatviewwidget.h"

#include <ChatList.h>
#include <IChatItem.h>
#include <Utility.h>

#include <QItemDelegate>
#include <QScrollBar>

ChatViewWidget::ChatViewWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewWidget)
{
    ui->setupUi(this);

    mChatListModel = new ChatList(this);
    ui->chatView->lower();
    ui->chatView->setModel(mChatListModel);

    auto itemDelegate = new ChatItemDelegate(this);
    ui->chatView->setItemDelegate(itemDelegate);

    // 每次滚动时，更新items
    // connect(ui->chatView->verticalScrollBar(), &QScrollBar::valueChanged, [=] {
    //     updateViewport();
    // });

    connect(ui->chatInputer, &ChatInputBox::send, mChatListModel,
        static_cast<void (ChatList::*)(const QString&)>(&ChatList::sendChat));
}

ChatViewWidget::~ChatViewWidget()
{
    delete ui;
}

void ChatViewWidget::resizeEvent(QResizeEvent* e)
{
    Q_UNUSED(e)

    updateViewport();
}

void ChatViewWidget::updateViewport()
{
    // 垂直扫描item并更新它们的尺寸
    /*for (int scan = 1; scan <= height();) {
        const auto i = (itemAt(width() / 2, scan));
        const auto chat = static_cast<IChatItem*>(itemWidget(i));

        if (chat) {
            chat->setFixedWidth(width());
            chat->updateContents();
            i->setSizeHint(chat->size());
        }

        // 每次增加1个间隔，再-1是为了确保一定能命中一个item
        scan += this->spacing() - 1;
    }*/
}
