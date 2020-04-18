#include "Navigation.h"
#include "ui_Navigation.h"

#include <User.h>
#include <Utility.h>

#include <QButtonGroup>
#include <QPushButton>

Navigation::Navigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Navigation)
{
    ui->setupUi(this);

    mButtonGroup = new QButtonGroup(this);
    connect(mButtonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &Navigation::navigate);
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
    btn->setIconSize(QSize(30, 30));

#ifndef QT_NO_TOOLTIP
    btn->setToolTip(btn->text());
#endif

    ui->navBtnsLayout->addWidget(btn);
    mButtonGroup->addButton(btn, mButtonGroup->buttons().length());
}

void Navigation::navigate(int index)
{
    int len = mButtonGroup->buttons().length();
    if (index >= 0 && index < len) {
        mButtonGroup->button(index)->toggle();
        switchIconColorForButton(mButtonGroup->button(index), "#4D7CFE");

        for (int j = 0; j < len; j++) {
            if (j != index) {
                switchIconColorForButton(mButtonGroup->button(j), "#A7ADBD");
            }
        }

        emit navigated(index);
    }
}

void Navigation::setUser(User* user) const
{
    ui->avatar->setOnlineState(user->getOnlineState());
    ui->lbUserName->setText(user->getNickName());
}

int Navigation::getCurrentNavigationIndex()
{
    return mButtonGroup->checkedId();
}

void Navigation::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    USE_STYLE_SHEET
}
