#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/Navigation.h"

#include <QPushButton>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Navigation *nav = new Navigation(this);

    nav->addNavButton("Message");
    nav->addNavButton("Friend");
    nav->addNavButton("Mine");
    nav->addNavButton("Settings");

    this->layout()->addWidget(nav);
}

MainWindow::~MainWindow()
{
    delete ui;
}

