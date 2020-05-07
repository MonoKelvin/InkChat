#include "ChatInputBox.h"

#include <AbstractChatListItem.h>
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

    QGridLayout *gridLayout = new QGridLayout(this);

    mChatInputer = new QPlainTextEdit(this);
    mChatInputer->setObjectName(QStringLiteral("chatInputer"));
    mChatInputer->setCursor(Qt::PointingHandCursor);
    mChatInputer->setPlaceholderText(tr("输入你想说的话..."));
    gridLayout->addWidget(mChatInputer, 1, 0, 1, 4);

    mBtnText = new QPushButton(this);
    mBtnText->setCheckable(true);
    mBtnText->setChecked(true);
    STD_ICON_SETTER(mBtnText, "edit");
    gridLayout->addWidget(mBtnText, 0, 0, 1, 1);

    mBtnEmoji = new QPushButton(this);
    STD_ICON_SETTER(mBtnEmoji, "emoji");
    gridLayout->addWidget(mBtnEmoji, 0, 1, 1, 1);

    mBtnFile = new QPushButton(this);
    STD_ICON_SETTER(mBtnFile, "file");
    gridLayout->addWidget(mBtnFile, 0, 2, 1, 1);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hSpacer, 0, 3, 1, 1);

    mBtnSend = new QPushButton(this->parentWidget());
    mBtnSend->setObjectName(QStringLiteral("btnSend"));
    SHADOW_ICON_SETTER(mBtnSend, "send", XTheme.getPrimarySubColor(1, 120));
    mBtnSend->raise();

    connect(mBtnText, &QPushButton::toggled, this, &ChatInputBox::onExpand);
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

void ChatInputBox::onExpand(bool enabled)
{
    if (enabled) {
        setFixedHeight(_oldH);

        mBtnSend->setVisible(true);
        mChatInputer->setVisible(true);
        mBtnEmoji->setEnabled(true);

        moveEvent(nullptr);
    } else {
        setFixedHeight(mChatInputer->y());

        // 设置控件状态
        mBtnSend->setVisible(false);
        mChatInputer->setVisible(false);
        mBtnEmoji->setEnabled(false);

        parentWidget()->adjustSize();
    }

    emit expand(enabled);
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
