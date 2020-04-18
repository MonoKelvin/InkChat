#include "ChatInputBox.h"

#include <Configuation.h>
#include <Utility.h>

#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QSpacerItem>

ChatInputBox::ChatInputBox(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(150);
    setAutoFillBackground(true);

    const QSize iconSize(20, 20);

    QGridLayout *gridLayout = new QGridLayout(this);

    mChatInputer = new QPlainTextEdit(this);
    mChatInputer->setObjectName(QStringLiteral("chatInputer"));
    mChatInputer->setCursor(Qt::PointingHandCursor);
    mChatInputer->setPlaceholderText(tr("输入你想说的话..."));
    gridLayout->addWidget(mChatInputer, 1, 0, 1, 6);

    mBtnText = new QPushButton(this);
    mBtnText->setObjectName(QStringLiteral("btnText"));
    mBtnText->setCursor(Qt::PointingHandCursor);
    mBtnText->setIcon(QIcon::fromTheme(QStringLiteral("edit")));
    mBtnText->setIconSize(iconSize);
    gridLayout->addWidget(mBtnText, 0, 0, 1, 1);

    mBtnImage = new QPushButton(this);
    mBtnImage->setObjectName(QStringLiteral("btnImage"));
    mBtnImage->setCursor(Qt::PointingHandCursor);
    mBtnImage->setIcon(QIcon::fromTheme(QStringLiteral("image")));
    mBtnImage->setIconSize(iconSize);
    gridLayout->addWidget(mBtnImage, 0, 1, 1, 1);

    mBtnVideo = new QPushButton(this);
    mBtnVideo->setObjectName(QStringLiteral("btnVideo"));
    mBtnVideo->setCursor(Qt::PointingHandCursor);
    mBtnVideo->setIcon(QIcon::fromTheme(QStringLiteral("video")));
    mBtnVideo->setIconSize(iconSize);
    gridLayout->addWidget(mBtnVideo, 0, 2, 1, 1);

    mBtnFile = new QPushButton(this);
    mBtnFile->setObjectName(QStringLiteral("btnFile"));
    mBtnFile->setCursor(Qt::PointingHandCursor);
    mBtnFile->setIcon(QIcon::fromTheme(QStringLiteral("file")));
    mBtnFile->setIconSize(iconSize);
    gridLayout->addWidget(mBtnFile, 0, 3, 1, 1);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hSpacer, 0, 4, 1, 1);

    mBtnExpand = new QPushButton(this);
    mBtnExpand->setCheckable(true);
    mBtnExpand->setObjectName(QStringLiteral("btnExpand"));
    mBtnExpand->setCursor(Qt::PointingHandCursor);
    mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("down")));
    mBtnExpand->setIconSize(iconSize);
    gridLayout->addWidget(mBtnExpand, 0, 5, 1, 1);

    mBtnSend = new QPushButton(this->parentWidget());
    mBtnSend->setObjectName(QStringLiteral("btnSend"));
    mBtnSend->setCursor(Qt::PointingHandCursor);
    mBtnSend->setFixedSize(40, 40);
    mBtnSend->setIcon(QIcon::fromTheme(QStringLiteral("send")));
    mBtnSend->setIconSize(QSize(24, 24));
    mBtnSend->raise();
    attachShadowEffect(mBtnSend, 0, 5.0, 20.0, QColor("#4D7CFE").lighter(120));

    connect(mBtnExpand, &QPushButton::toggled, this, &ChatInputBox::onFoldup);
    connect(mBtnSend, &QPushButton::clicked, [this] {
        if (mChatInputer->toPlainText().isEmpty()) {
            return;
        }

        emit send(mChatInputer->toPlainText());

        mChatInputer->clear();
    });

    moveEvent(nullptr);
    _oldH = height();
}

void ChatInputBox::onFoldup(bool enabled)
{
    if (enabled) {
        setFixedHeight(mChatInputer->y());

        // 设置控件状态
        mBtnSend->setVisible(false);
        mChatInputer->setVisible(false);
        mBtnText->setEnabled(false);
        mBtnImage->setEnabled(false);
        mBtnVideo->setEnabled(false);
        mBtnFile->setEnabled(false);
        mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("up")));

        parentWidget()->adjustSize();
    } else {
        setFixedHeight(_oldH);

        mBtnSend->setVisible(true);
        mChatInputer->setVisible(true);
        mBtnText->setEnabled(true);
        mBtnImage->setEnabled(true);
        mBtnVideo->setEnabled(true);
        mBtnFile->setEnabled(true);
        mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("down")));

        moveEvent(nullptr);
    }

    emit foldup(enabled);
}

void ChatInputBox::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event)

    mBtnSend->move(geometry().right() - mBtnSend->width() - ESpacing::Std,
        y() - mBtnSend->height() - ESpacing::Std);
}

void ChatInputBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    USE_STYLE_SHEET
}
