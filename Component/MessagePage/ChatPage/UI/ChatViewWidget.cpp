#include "ChatViewWidget.h"
#include "ui_chatviewwidget.h"

#include <ChatList.h>
#include <ChatItem.h>
#include <Utility.h>

#include <QScrollBar>

ChatViewWidget::ChatViewWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewWidget)
{
    ui->setupUi(this);

    mChatListModel = new ChatList(this);

    ui->chatView->setItemDelegate(new ChatItemDelegate(this));
    ui->chatView->setModel(mChatListModel);

    connect(ui->chatInputer, &ChatInputBox::send, this, &ChatViewWidget::sendMessage);
    connect(ui->chatInputer, &ChatInputBox::foldup, [this] {
        const auto& sp = ui->chatView->spacing();
        QModelIndex index;

        // 从上面依次搜索找到第一个item
        for (int i = 2 * sp; i >= sp; i--) {
            index = ui->chatView->indexAt(QPoint(sp + 1, i));
            if (index.isValid()) {
                // 如果用户浏览倒数第4条之前的聊天记录就不要滚动到最低部
                if (index.row() < mChatListModel->rowCount() - 4) {
                    return;
                }
                break;
            }
        }

        ui->chatView->scrollToBottom();
    });

    // 调整显示层
    ui->chatView->lower();
    lower();
}

ChatViewWidget::~ChatViewWidget()
{
    delete ui;
}

QListView* ChatViewWidget::getChatView() const
{
    return ui->chatView;
}

void ChatViewWidget::resizeEvent(QResizeEvent* event)
{
    updateLayout(ui->chatView, true, true);
}

void ChatViewWidget::sendMessage(const QString& msg)
{
    mChatListModel->sendChat(msg);
    ui->chatView->scrollToBottom();
    ui->chatView->updateGeometry();
}
