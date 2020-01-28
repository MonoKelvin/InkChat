#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/Utility.h"
#include "Core/User.h"
#include "Core/Widget/MessageItem.h"
#include "Core/Widget/FriendInfoCard.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->msgListTitle->setObjectName(QStringLiteral("title"));

    ui->navigation->addNavButton("Message");
    ui->navigation->addNavButton("Friend");
    ui->navigation->addNavButton("Mine");
    ui->navigation->addNavButton("Settings");
    ui->navigation->navigate(0);

    auto mi = new MessageItem(1, ui->messageList);
    mi->setName(QStringLiteral("Tony Stack"));
    mi->setMessage(QStringLiteral("There was a message yestoday"));
    mi->setUnreadMessageNumber(20);

    auto mi2 = new MessageItem(2, ui->messageList);
    mi2->setName(QStringLiteral("Francis King"));
    mi2->setUnreadMessageNumber(150);
    mi2->setMessage("昨天发来的100多条消息。昨天发来的100多条消息。");

    auto mi3 = new MessageItem(2, ui->messageList);
    mi3->setName(QStringLiteral("Jerry McKenzie"));
    mi3->setMessage(QStringLiteral("Lorem ipsum dolor sit amet, consectetur adipisicing elit,"));

    // 测试
    connect(ui->msgSearchBox, &QLineEdit::returnPressed, ui->chatView, &ChatListView::clearChats);

    // 当聊天视图滚动到最底端时，又展开输入框后，会使得视图布局不更新，导致无法看到最后几条消息。
    connect(ui->chatInputer, &ChatInputBox::onFoldup, [=] {
        updateLayout(this);
        const auto vsb = ui->chatView->verticalScrollBar();
        if (vsb->value() > vsb->maximum() - ui->chatInputer->height()) {
            ui->chatView->scrollToBottom();
        }
    });

//    auto fc = new FriendInfoCard(User::GetUnloginUser(), this);
//    fc->setFixedSize(250, height());
}

MainWindow::~MainWindow()
{
    User::DestroyMe();
    delete ui;
}
