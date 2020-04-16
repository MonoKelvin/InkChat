#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <MessageDatabase.h>
#include <MessageList.h>
#include <Utility.h>

#include <QListView>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1080, 640);

    ui->msgTitle->setObjectName(QStringLiteral("title"));

    ui->navigation->addNavButton(QIcon::fromTheme("message"));
    ui->navigation->addNavButton(QIcon::fromTheme("person"));
    ui->navigation->navigate(0);

    mMessageListModel = new MessageList(this);
    ui->messageList->setModel(mMessageListModel);

    // 检测局域网
    MessageDatabase::Instance()->detectLanEnvironment(mMessageListModel);

    // 当聊天视图滚动到最底端时，又展开输入框后，会使得视图布局不更新，导致无法看到最后几条消息。
    // connect(ui->chatInputer, &ChatInputBox::onFoldup, [=] {
    //     updateLayout(this);
    //     const auto vsb = ui->chatView->verticalScrollBar();
    //     if (vsb->value() > vsb->maximum() - ui->chatInputer->height()) {
    //         ui->chatView->scrollToBottom();
    //     }
    // });

    connect(ui->navigation, &Navigation::onNavigated, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}
