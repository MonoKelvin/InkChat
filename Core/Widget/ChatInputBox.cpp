﻿#include "ChatInputBox.h"

#include "../Configuation.h"
#include "../Utility/Utility.h"

#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QSpacerItem>

ChatInputBox::ChatInputBox(QWidget *parent)
    : QWidget(parent)
    , mDisplayMode(EDisplayMode::Expand)
{
    setFixedHeight(150);

    mBtnSend = new QPushButton(this->parentWidget());
    mBtnSend->setObjectName(QLatin1String("btnSend"));
    mBtnSend->setCursor(Qt::PointingHandCursor);
    mBtnSend->setFixedSize(40, 40);
    attachShadowEffect(mBtnSend, 0.0, 4.0, 20.0, "#A7ADBD");

    QGridLayout *gridLayout = new QGridLayout(this);

    mChatInputer = new QPlainTextEdit(this);
    mChatInputer->setObjectName(QLatin1String("chatInputer"));
    mChatInputer->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mChatInputer, 1, 0, 1, 6);

    mBtnText = new QPushButton(this);
    mBtnText->setObjectName(QLatin1String("btnText"));
    mBtnText->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mBtnText, 0, 0, 1, 1);

    mBtnImage = new QPushButton(this);
    mBtnImage->setObjectName(QLatin1String("btnImage"));
    mBtnImage->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mBtnImage, 0, 1, 1, 1);

    mBtnVideo = new QPushButton(this);
    mBtnVideo->setObjectName(QLatin1String("btnVideo"));
    mBtnVideo->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mBtnVideo, 0, 2, 1, 1);

    mBtnFile = new QPushButton(this);
    mBtnFile->setObjectName(QLatin1String("btnFile"));
    mBtnFile->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mBtnFile, 0, 3, 1, 1);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hSpacer, 0, 4, 1, 1);

    mBtnExpand = new QPushButton(this);
    mBtnExpand->setCheckable(true);
    mBtnExpand->setObjectName(QLatin1String("btnExpand"));
    mBtnExpand->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(mBtnExpand, 0, 5, 1, 1);

    connect(mBtnExpand, &QPushButton::toggled, this, &ChatInputBox::foldup);

    moveEvent(nullptr);
    _oldH = height();
}

void ChatInputBox::foldup(bool enabled)
{
    if (enabled && mDisplayMode == EDisplayMode::Expand) {
        setFixedHeight(mChatInputer->y());
        mDisplayMode = EDisplayMode::Foldup;

        // 隐藏控件
        mBtnSend->setVisible(false);
        mChatInputer->setVisible(false);

        // 取消激活按钮
        mBtnText->setEnabled(false);
        mBtnImage->setEnabled(false);
        mBtnVideo->setEnabled(false);
        mBtnFile->setEnabled(false);

        parentWidget()->adjustSize();
    } else if (!enabled && mDisplayMode != EDisplayMode::Expand) {
        setFixedHeight(_oldH);
        mDisplayMode = EDisplayMode::Expand;

        mBtnSend->setVisible(true);
        mChatInputer->setVisible(true);

        mBtnText->setEnabled(true);
        mBtnImage->setEnabled(true);
        mBtnVideo->setEnabled(true);
        mBtnFile->setEnabled(true);

        moveEvent(nullptr);
    }

    emit onFoldup(enabled);
}

void ChatInputBox::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event)

    mBtnSend->move(geometry().right() - mBtnSend->width() - ESpacing::Wide,
                   y() - mBtnSend->height() - ESpacing::Wide);
}
