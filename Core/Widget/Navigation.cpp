#include "Navigation.h"

#include "../Utility/Utility.h"
#include "../User.h"
#include "Avatar.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QScrollArea>

Navigation::Navigation(QWidget *parent)
    : QWidget(parent)
    , mOrientation(EOrientation::Vertical)
    , mNavButtonsAlignment(EAlignment::Center)
{
    init();
}

Navigation::Navigation(EAlignment alignment, EOrientation orientation, QWidget *parent)
    : QWidget(parent)
    , mOrientation(orientation)
    , mNavButtonsAlignment(alignment)
{
    init();
    setNavButtonsAlignment(alignment);
}

void Navigation::init()
{
    mButtonGroup = new QButtonGroup(this);
    mButtonGroup->setExclusive(true);

    mNavContents = new QWidget(this);
    mNavContents->setObjectName(QLatin1String("navBtnsContents"));

    QBoxLayout *layout;
    QBoxLayout *navBtnsLayout;
    if (EOrientation::Vertical == mOrientation)
    {
        layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
        navBtnsLayout = new QBoxLayout(QBoxLayout::TopToBottom, mNavContents);
    }
    else
    {
        layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
        navBtnsLayout = new QBoxLayout(QBoxLayout::LeftToRight, mNavContents);
    }
    layout->setSpacing(0);
    layout->setMargin(0);

    navBtnsLayout->setSpacing(ESpacing::Narrow);
    navBtnsLayout->setMargin(0);
    navBtnsLayout->addStretch(1);
    navBtnsLayout->addStretch(1);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setWidget(mNavContents);
    layout->addWidget(scrollArea);

#define BG_CAST void (QButtonGroup::*)(int)
#define BG_SIGNAL(_Func_) static_cast<BG_CAST>(&QButtonGroup::_Func_)
    connect(mButtonGroup, BG_SIGNAL(buttonClicked), [=](int i) {
        emit onNavigated(i);
    });
#undef BG_SIGNAL
#undef BG_CAST
}

void Navigation::setPadding(int margin)
{
    this->layout()->setMargin(margin);
}

void Navigation::setPadding(int left, int top, int right, int bottom)
{
    this->layout()->setContentsMargins(left, top, right, bottom);
}

void Navigation::addNavButton(const QString &text, const QIcon &icon)
{
    QPushButton *btn = new QPushButton(icon, text, mNavContents);

    btn->setCheckable(true);
    btn->setObjectName(QLatin1String("navButton"));
    btn->setCursor(Qt::PointingHandCursor);

#ifndef QT_NO_TOOLTIP
    btn->setToolTip(btn->text());
#endif

    const auto layout = static_cast<QBoxLayout *>(mNavContents->layout());
    switch (mNavButtonsAlignment)
    {
    case EAlignment::Top:
        break;
    case EAlignment::Center:
    default:
        layout->insertWidget(layout->count() - 1, btn, 1);
        break;
    }
    mButtonGroup->addButton(btn, mButtonGroup->buttons().length());

    resizeNavButtonContents();
}

void Navigation::navigate(int index)
{
    if (index < mButtonGroup->buttons().length()) {
        mButtonGroup->button(index)->click();
    }
}

void Navigation::setNavButtonSize(const QSize &size)
{
    for (auto i : mButtonGroup->buttons())
    {
        i->setFixedSize(size);
    }

    resizeNavButtonContents();
}

void Navigation::setNavButtonSpacing(int spacing)
{
    mNavContents->layout()->setSpacing(spacing);
}

void Navigation::setNavButtonsMargin(int margin)
{
    mNavContents->layout()->setMargin(margin);
}

void Navigation::setNavButtonsMargin(int left, int top, int right, int bottom)
{
    mNavContents->layout()->setContentsMargins(left, top, right, bottom);
}

void Navigation::setUser(QScopedPointer<User> user)
{
    Avatar *avatar = new Avatar(user->getAvatar(), this);
    avatar->setOnlineState(user->getOnlineState());
    this->layout()->addWidget(avatar);
}

int Navigation::getCurrentNavigationIndex()
{
    return mButtonGroup->checkedId();
}

void Navigation::setNavButtonsAlignment(const EAlignment &alignment)
{
    if (mNavButtonsAlignment == alignment)
    {
        return;
    }

    const auto layout = static_cast<QBoxLayout *>(mNavContents->layout());
    switch (alignment)
    {
    case EAlignment::Top:
    case EAlignment::Left:
        layout->setStretch(0, 0);
        layout->setStretch(layout->count() - 1, 1);
        break;
    case EAlignment::Bottom:
    case EAlignment::Right:
        layout->setStretch(0, 1);
        layout->setStretch(layout->count() - 1, 0);
        break;
    case EAlignment::Center:
        layout->setStretch(0, 0);
        layout->setStretch(layout->count() - 1, 0);
        break;
    }
}

void Navigation::resizeNavButtonContents()
{
    int h = 0;
    for (auto i : mButtonGroup->buttons())
    {
        h += i->height();
    }
    h += mNavContents->layout()->spacing() * mButtonGroup->buttons().length();

    mNavContents->setMinimumSize(mNavContents->parentWidget()->width(), h);
}
