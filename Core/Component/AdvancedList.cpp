#include "AdvancedList.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>

AdvancedList::AdvancedList(QWidget *parent)
    : QWidget(parent)
    , mBackTopAlignment(EAlignment::Right)
{
    mToopContainer = new QWidget(this);
    mListWidget = new QListWidget(this);
    mBtnRefresh = new QPushButton(this);
    mSearchBox = new QLineEdit(mToopContainer);
    mBtnBackTop = new QPushButton(this);

    QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, mToopContainer);

    mToopContainer->setObjectName(QLatin1String("listToolContainer"));
    mBtnBackTop->setObjectName(QLatin1String("backTopBtn"));
    mBtnRefresh->setObjectName(QLatin1String("refreshBtn"));
    mSearchBox->setObjectName(QLatin1String("searchBox"));

    hLayout->addWidget(mBtnRefresh);
    hLayout->addWidget(mSearchBox);

#if QT_CONFIG(menu)
    mBtnMenu = new QPushButton(mToopContainer);
    hLayout->addWidget(mBtnMenu);
#endif

    hLayout->setMargin(0);
    hLayout->setSpacing(ESpacing::Std);

    vLayout->addWidget(mToopContainer);
    vLayout->addWidget(mListWidget);
    vLayout->setSpacing(0);
    vLayout->setMargin(0);

    setMinimumWidth(mBtnBackTop->width() + 2 * ESpacing::Std);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setMaximumWidth(360);

    mListWidget->setHorizontalScrollMode(QListView::ScrollPerPixel);
    mListWidget->setVerticalScrollMode(QListView::ScrollPerPixel);
    mListWidget->setEditTriggers(QListView::NoEditTriggers);
    mListWidget->setResizeMode(QListWidget::Adjust);
    mListWidget->setSpacing(0);

}

void AdvancedList::addListItem(QWidget *widget)
{
    widget->setParent(this);

    QListWidgetItem *item = new QListWidgetItem();
    mListWidget->addItem(item);
    mListWidget->setItemWidget(item, widget);
}

void AdvancedList::clearList()
{
    while (nullptr != mListWidget->item(0))
    {
        auto i = mListWidget->item(0);
        mListWidget->removeItemWidget(i);
        delete i;
        i = nullptr;
    }
}

void AdvancedList::closeSearchBox()
{
}

void AdvancedList::expandSearchBox()
{
}

void AdvancedList::search(const QString &key, bool isRealTime)
{
}

bool AdvancedList::refresh()
{
}

void AdvancedList::resizeEvent(QResizeEvent *e)
{
    int x = 0;
    int y = height() - mBtnBackTop->height() - ESpacing::Std;

    switch (mBackTopAlignment)
    {
    case EAlignment::Center:
        x = (width() - mBtnBackTop->width()) / 2;
        break;
    case EAlignment::Left:
        x = ESpacing::Std;
        break;
    case EAlignment::Right:
    default:
        x = width() - mBtnBackTop->width() - ESpacing::Std;
        break;
    }
    mBtnBackTop->move(x, y);

    return QWidget::resizeEvent(e);
}
