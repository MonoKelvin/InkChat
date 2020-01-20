#include "Navigation.h"

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
//    this->setObjectName("navigation");
    setMinimumSize(NAV_BUTTON_BASE_SIZE, NAV_BUTTON_BASE_SIZE);
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mButtonGroup = new QButtonGroup(this);
    mButtonGroup->setExclusive(true);

    QScrollArea *scrollArea;
    QBoxLayout *layout;
    QBoxLayout *navBtnsLayout;

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    mNavContents = new QWidget();
    mNavContents->setObjectName(QLatin1String("navBtnsContents"));

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

//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    navBtnsLayout->setSpacing(ESpacing::Wide);
    navBtnsLayout->setMargin(0);
    layout->setSpacing(0);
    layout->setMargin(0);

    navBtnsLayout->addStretch(1);
    navBtnsLayout->addStretch(1);

    scrollArea->setWidget(mNavContents);
    layout->addWidget(scrollArea);

    connect(mButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &Navigation::onNavigated);
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
