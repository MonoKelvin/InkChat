#include "SearchableList.h"

#include "../Configuation.h"
#include "../Utility/Utility.h"

#include <QPushButton>

SearchableList::SearchableList(QWidget *parent)
    : QListWidget(parent)
{
    setAutoFillBackground(true);
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListView::Adjust);
    setSpacing(0);

    mBackTop = new QPushButton(this);
    mBackTop->setObjectName(QLatin1String("btnBackTop"));
    mBackTop->setCursor(Qt::PointingHandCursor);
    mBackTop->setFixedSize(40, 40);
    attachShadowEffect(mBackTop, 0.0, 4.0, 20.0, "#A7ADBD");

    resize(size());

    connect(mBackTop, &QPushButton::clicked, this, &SearchableList::scrollToTop);
}

void SearchableList::setBackTopVisiable(bool enabled)
{
    mBackTop->setVisible(enabled);
}

void SearchableList::search(const QString &key)
{
    scrollToTop();

    const int c = count();

    for (int index = 0; index < c; index++) {
        const auto i = item(index);
        if (!searchDelegate(key, i)) {
            i->setHidden(true);
        }
    }
}


void SearchableList::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    if(mBackTop->isVisible()) {
        switch (mBackTopAlignment) {
        case EAlignment::Right:
            mBackTop->move(width() - mBackTop->width() - ESpacing::Wide,
                           height() - mBackTop->height() - ESpacing::Wide);
            break;
        case EAlignment::Left:
            mBackTop->move(ESpacing::Wide, height() - mBackTop->height() - ESpacing::Wide);
            break;
        default:
            mBackTop->move((width() - mBackTop->width()) / 2,
                           height() - mBackTop->height() - ESpacing::Wide);
            break;
        }
    }
}
