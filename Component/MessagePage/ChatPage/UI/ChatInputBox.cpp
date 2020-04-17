#include "ChatInputBox.h"

#include <Configuation.h>
#include <Utility.h>

#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QSpacerItem>

ChatInputBox::ChatInputBox(QWidget *parent)
    : QWidget(parent)
    , mDisplayMode(EDisplayMode::Expand)
{
    setFixedHeight(150);
    setAutoFillBackground(true);

    const QSize iconSize(24, 24);

    // mBtnSend = new QPushButton(this->parentWidget());
    mBtnSend = new QPushButton(this);
    mBtnSend->setObjectName(QStringLiteral("btnSend"));
    mBtnSend->setCursor(Qt::PointingHandCursor);
    mBtnSend->setFixedSize(40, 40);
    mBtnSend->setIcon(QIcon::fromTheme(QStringLiteral("send")));
    mBtnSend->setIconSize(QSize(30, 30));
    mBtnSend->raise();

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

    connect(mBtnExpand, &QPushButton::toggled, this, &ChatInputBox::onFoldup);
    connect(mBtnSend, &QPushButton::clicked, [this] { emit send(mChatInputer->toPlainText()); });

    moveEvent(nullptr);
    _oldH = height();
}

void ChatInputBox::onFoldup(bool enabled)
{
    if (enabled && mDisplayMode == EDisplayMode::Expand) {
        setFixedHeight(mChatInputer->y());
        mDisplayMode = EDisplayMode::Foldup;

        // 设置控件状态
        mBtnSend->setVisible(false);
        mChatInputer->setVisible(false);
        mBtnText->setEnabled(false);
        mBtnImage->setEnabled(false);
        mBtnVideo->setEnabled(false);
        mBtnFile->setEnabled(false);
        mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("up")));

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
        mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("down")));

        moveEvent(nullptr);
    }

    emit foldup(enabled);
}

void ChatInputBox::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event)

    mBtnSend->move(geometry().right() - mBtnSend->width() - ESpacing::Wide,
        height() - mBtnSend->height() - ESpacing::Wide);
}

void ChatInputBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    USE_STYLE_SHEET
}
