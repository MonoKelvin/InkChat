#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/Utility/Utility.h"
#include "Core/User.h"
#include "Core/Widget/MessageItem.h"
#include "Core/Widget/FriendInfoCard.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->navigation->addNavButton("Message");
    ui->navigation->addNavButton("Friend");
    ui->navigation->addNavButton("Mine");
    ui->navigation->addNavButton("Settings");
    ui->navigation->navigate(1);

    auto mi = new MessageItem(1, ui->messageList);
    mi->setName(QLatin1String("Tony Stack"));
    mi->setMessage(QLatin1String("There was a message yestoday"));
    mi->setUnreadMessageNumber(20);

    auto mi2 = new MessageItem(2, ui->messageList);
    mi2->setName(QLatin1String("Francis King"));
    mi2->setUnreadMessageNumber(150);
    mi2->setMessage("昨天发来的100多条消息。昨天发来的100多条消息。");

    auto mi3 = new MessageItem(2, ui->messageList);
    mi3->setName(QLatin1String("Jerry McKenzie"));
    mi3->setMessage(QLatin1String("Lorem ipsum dolor sit amet, consectetur adipisicing elit,"));

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

    FriendInfoCard *fid = new FriendInfoCard(User::GetUnloginUser(), this);
    fid->setGeometry(0, 0, 250, 640);
}

MainWindow::~MainWindow()
{
    User::DestroyMe();
    delete ui;
}
