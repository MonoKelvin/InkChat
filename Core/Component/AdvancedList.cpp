#include "AdvancedList.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>

AdvancedList::AdvancedList(QWidget *parent)
    : QWidget(parent)
{
    mToopContainer = new QWidget(this);
    mListView = new QListView(this);
    mSearchBox = new QLineEdit(mToopContainer);
    mBtnMenu = new QPushButton(mToopContainer);
    //    mBtnBackToTop = new QPushButton(this);

    QBoxLayout *vLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QBoxLayout *hLayout = new QBoxLayout(QBoxLayout::LeftToRight, mToopContainer);

    mToopContainer->setObjectName(QString::fromUtf8("listToolContainer"));

    hLayout->addWidget(mSearchBox);
    hLayout->addWidget(mBtnMenu);
    hLayout->setMargin(0);
    hLayout->setSpacing(ESpacing::Std);

    vLayout->addWidget(mToopContainer);
    vLayout->addWidget(mListView);
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

}
