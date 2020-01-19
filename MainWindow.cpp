#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Core/Component/Navigation.h"
#include "Core/Component/AdvancedList.h"

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

    AdvancedList *adl = new AdvancedList(this);

    layout()->addWidget(nav);
    layout()->addWidget(adl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

