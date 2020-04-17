#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <MessageDatabase.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <User.h>
#include <Utility.h>
#include <Widget/PromptWidget.h>

#include <QListView>
#include <QMenu>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1080, 640);

    //ui->chatView->lower();
    ui->messageList->raise();
    ui->msgTitle->raise();
    ui->navigation->raise();
    // attachShadowEffect(ui->messageList, 1.0);
    // attachShadowEffect(ui->navigation, 1.0);

    // 导航栏
    ui->navigation->addNavButton(QIcon::fromTheme("message"));
    ui->navigation->addNavButton(QIcon::fromTheme("person"));
    ui->navigation->setUser(User::Instance());
    ui->navigation->navigate(0);

    // 消息列表
    mMessageListModel = new MessageList(this);
    ui->messageList->setItemDelegate(new MessageItemDelegate(this)); // 设置代理
    ui->messageList->setModel(mMessageListModel); // 设置模型

    // 加载缓存消息
    mMessageListModel->load();

    // 当聊天视图滚动到最底端时，又展开输入框后，会使得视图布局不更新，导致无法看到最后几条消息。
    // connect(ui->chatInputer, &ChatInputBox::onFoldup, [=] {
    //     updateLayout(this);
    //     const auto vsb = ui->chatView->verticalScrollBar();
    //     if (vsb->value() > vsb->maximum() - ui->chatInputer->height()) {
    //         ui->chatView->scrollToBottom();
    //     }
    // });

    connect(ui->navigation, &Navigation::navigated, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(mMessageListModel, &MessageList::failed, this, &MainWindow::onFailed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFailed(const QString& msg)
{
    new PromptWidget(msg, this);
}
