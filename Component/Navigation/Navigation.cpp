#include "Navigation.h"
#include "ui_Navigation.h"

#include <User.h>

#include <QButtonGroup>
#include <QPushButton>

Navigation::Navigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Navigation)
{
    ui->setupUi(this);

    mButtonGroup = new QButtonGroup(this);

    connect(mButtonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [this](int i) {
        emit onNavigated(i);
    });
}

Navigation::~Navigation()
{
    delete ui;
}

void Navigation::addNavButton(const QIcon& icon, const QString& text)
{
    QPushButton* btn = new QPushButton(icon, text, this);

    btn->setCheckable(true);
    btn->setObjectName(QStringLiteral("navButton"));
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedSize(40, 40);

#ifndef QT_NO_TOOLTIP
    btn->setToolTip(btn->text());
#endif

    ui->navBtnsLayout->addWidget(btn);
    mButtonGroup->addButton(btn, mButtonGroup->buttons().length());
}

void Navigation::navigate(int index)
{
    if (index < mButtonGroup->buttons().length()) {
        mButtonGroup->button(index)->click();
    }
}

void Navigation::setUser(QScopedPointer<User> user)
{
    ui->avatar->setOnlineState(user->getOnlineState());
    ui->lbUserName->setText(user->getNickName());
}

int Navigation::getCurrentNavigationIndex()
{
    return mButtonGroup->checkedId();
}

/*
void Navigation::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    USE_STYLE_SHEET
}
*/
