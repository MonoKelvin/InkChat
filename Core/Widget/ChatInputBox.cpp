#include "ChatInputBox.h"

#include "../Configuation.h"

#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QSpacerItem>

ChatInputBox::ChatInputBox(QWidget *parent)
    : QWidget(parent)
{
    mBtnSend = new QPushButton(this);
    mBtnSend->setFixedSize(40, 40);

    mBtnExpand = new QPushButton(this);
    mBtnExpand->setFixedSize(40, 40);

    QGridLayout *gridLayout = new QGridLayout(this);

    mBtnText = new QPushButton(this);
    mBtnText->setObjectName(QLatin1String("btnText"));
    gridLayout->addWidget(mBtnText, 0, 0, 1, 1);

    mChatInputer = new QPlainTextEdit(this);
    mChatInputer->setObjectName(QLatin1String("chatInputer"));
    gridLayout->addWidget(mChatInputer, 1, 0, 1, 5);

    mBtnFile = new QPushButton(this);
    mBtnFile->setObjectName(QLatin1String("btnFile"));
    gridLayout->addWidget(mBtnFile, 0, 3, 1, 1);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hSpacer, 0, 4, 1, 1);

    mBtnVideo = new QPushButton(this);
    mBtnVideo->setObjectName(QLatin1String("btnVideo"));
    gridLayout->addWidget(mBtnVideo, 0, 2, 1, 1);

    mBtnImage = new QPushButton(this);
    mBtnImage->setObjectName(QLatin1String("btnImage"));
    gridLayout->addWidget(mBtnImage, 0, 1, 1, 1);

    resizeEvent(nullptr);
}

void ChatInputBox::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    mBtnSend->move(this->y() - ESpacing::Narrow, geometry().right() - mBtnSend->width() - ESpacing::Std);
    mBtnExpand->move(geometry().right() - mBtnExpand->width() - ESpacing::Std,
                     geometry().bottom() - mBtnExpand->height() - ESpacing::Std);
}
