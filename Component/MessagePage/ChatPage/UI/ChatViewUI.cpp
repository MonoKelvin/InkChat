#include "ChatViewUI.h"

#include <Configuation.h>

#include <QListWidgetItem>

#include <QLabel>
#include <QPushButton>
#include <QScrollBar>

ChatViewUI::ChatViewUI(QWidget* parent)
    : QListView(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListView::Adjust);
    setSpacing(ESpacing::Narrow);

    mHeaderTitle = new QLabel(this);
    mBtnTool = new QPushButton(this);

    mHeaderTitle->setAlignment(Qt::AlignCenter);
    mHeaderTitle->setObjectName(QStringLiteral("header"));
    mBtnTool->setFixedSize(16, 16);
    mBtnTool->setObjectName(QStringLiteral("toolBtn"));

    // 每次滚动时，更新items
    /*connect(verticalScrollBar(), &QScrollBar::valueChanged, [=] {
        updateViewport();
    });*/

    resizeEvent(nullptr);
}

void ChatViewUI::setHeaderTitle(const QString& title)
{
    mHeaderTitle->setText(title);
}

void ChatViewUI::resizeEvent(QResizeEvent* e)
{
    Q_UNUSED(e)
    updateViewport();
}

void ChatViewUI::updateViewport()
{
    // 垂直扫描item并更新它们的尺寸
    /*for (int scan = 1; scan <= height();) {
        const auto i = (itemAt(width() / 2, scan));
        const auto chat = static_cast<IChatItem*>(itemWidget(i));

        if (chat) {
            chat->setFixedWidth(width());
            chat->updateContents();
            i->setSizeHint(chat->size());
        }

        // 每次增加1个间隔，再-1是为了确保一定能命中一个item
        scan += this->spacing() - 1;
    }*/
}
