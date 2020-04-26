#include "ChatInputBox.h"

#include <AppTheme.h>
#include <Utility.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
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
    gridLayout->addWidget(mChatInputer, 1, 0, 1, 5);

    mBtnImage = new QPushButton(this);
    mBtnImage->setObjectName(QStringLiteral("btnImage"));
    mBtnImage->setCursor(Qt::PointingHandCursor);
    mBtnImage->setIcon(QIcon::fromTheme(QStringLiteral("image")));
    mBtnImage->setIconSize(iconSize);
    gridLayout->addWidget(mBtnImage, 0, 0, 1, 1);

    mBtnVideo = new QPushButton(this);
    mBtnVideo->setObjectName(QStringLiteral("btnVideo"));
    mBtnVideo->setCursor(Qt::PointingHandCursor);
    mBtnVideo->setIcon(QIcon::fromTheme(QStringLiteral("video")));
    mBtnVideo->setIconSize(iconSize);
    gridLayout->addWidget(mBtnVideo, 0, 1, 1, 1);

    mBtnFile = new QPushButton(this);
    mBtnFile->setObjectName(QStringLiteral("btnFile"));
    mBtnFile->setCursor(Qt::PointingHandCursor);
    mBtnFile->setIcon(QIcon::fromTheme(QStringLiteral("file")));
    mBtnFile->setIconSize(iconSize);
    gridLayout->addWidget(mBtnFile, 0, 2, 1, 1);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hSpacer, 0, 3, 1, 1);

    mBtnExpand = new QPushButton(this);
    mBtnExpand->setCheckable(true);
    mBtnExpand->setObjectName(QStringLiteral("btnExpand"));
    mBtnExpand->setCursor(Qt::PointingHandCursor);
    mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("down")));
    mBtnExpand->setIconSize(iconSize);
    gridLayout->addWidget(mBtnExpand, 0, 4, 1, 1);

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
        const auto& msg = mChatInputer->toPlainText();
        if (!msg.isEmpty()) {
            emit send(AbstractChatListItem::Text, msg);
            mChatInputer->clear();
        }
    });
    connect(mBtnFile, &QPushButton::clicked, [this] {
        const QString& fileName = QFileDialog::getOpenFileName(nullptr, tr("选择要发送的文件"));
        if (!fileName.isEmpty()) {
            emit send(AbstractChatListItem::File, fileName);
        }
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
        mBtnImage->setEnabled(false);
        mBtnVideo->setEnabled(false);
        mBtnFile->setEnabled(false);
        mBtnExpand->setIcon(QIcon::fromTheme(QStringLiteral("up")));

        parentWidget()->adjustSize();
    } else {
        setFixedHeight(_oldH);

        mBtnSend->setVisible(true);
        mChatInputer->setVisible(true);
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

    mBtnSend->move(geometry().right() - mBtnSend->width() - ESize::Std,
        y() - mBtnSend->height() - ESize::Std);
}

void ChatInputBox::paintEvent(QPaintEvent*)
{
    USE_STYLE_SHEET
}
