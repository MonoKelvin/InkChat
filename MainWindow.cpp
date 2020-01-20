#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/User.h"
#include "Core/Component/Navigation.h"
#include "Core/Widget/MessageList.h"

#include <QLayout>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->navigation->addNavButton("Message");
    ui->navigation->addNavButton("Friend");
    ui->navigation->addNavButton("Mine");
    ui->navigation->addNavButton("Settings");

}

MainWindow::~MainWindow()
{
    User::DestroyMe();
    delete ui;
}

