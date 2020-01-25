#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/User.h"
#include "Core/Widget/MessageItem.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->navigation->addNavButton("Message");
    ui->navigation->addNavButton("Friend");
    ui->navigation->addNavButton("Mine");
    ui->navigation->addNavButton("Settings");

    auto mi = new MessageItem(1, ui->messageList);
    mi->setName(QLatin1String("Tony Stack"));
    mi->setMessage(QLatin1String("There was a message yestoday"));
    mi->setUnreadMessageNumber(20);
    ui->messageList->addMessage(mi);

    auto mi2 = new MessageItem(2, ui->messageList);
    mi2->setName(QLatin1String("Francis King"));
    mi2->setUnreadMessageNumber(150);
    ui->messageList->addMessage(mi2);

    auto mi3 = new MessageItem(2, ui->messageList);
    mi3->setName(QLatin1String("Jerry McKenzie"));
    mi3->setMessage(QLatin1String("Lorem ipsum dolor sit amet, consectetur adipisicing elit,"));
    ui->messageList->addMessage(mi3);
}

MainWindow::~MainWindow()
{
    User::DestroyMe();
    delete ui;
}

