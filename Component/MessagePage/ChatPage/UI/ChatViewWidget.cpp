#include "ChatViewWidget.h"
#include "ui_chatviewwidget.h"

#include <ChatList.h>
#include <IChatItem.h>
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

void ChatViewWidget::sendMessage(const QString& msg)
{
    mChatListModel->sendChat(msg);
    ui->chatView->scrollToBottom();
    ui->chatView->updateGeometry();
}
