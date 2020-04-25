#include "ChatViewWidget.h"
#include "ui_chatviewwidget.h"

#include <ChatItem.h>
#include <ChatList.h>
#include <Utility.h>
#include <Widget/LoadingIndicator.h>

#include <QMouseEvent>
#include <QScrollBar>

ChatViewWidget::ChatViewWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewWidget)
{
    ui->setupUi(this);

    mChatListModel = new ChatList(this);
    mLoader = nullptr;

    ui->chatView->setItemDelegate(new ChatItemDelegate(this));
    ui->chatView->setModel(mChatListModel);
    ui->chatView->viewport()->installEventFilter(this);
    ui->chatView->verticalScrollBar()->setPageStep(5);

    connect(ui->chatInputer, &ChatInputBox::send, this, &ChatViewWidget::sendMessage);
    connect(ui->chatInputer, &ChatInputBox::foldup, this, &ChatViewWidget::autoDetermineScrollToBottom);

    // 调整显示层
    ui->chatView->lower();
    lower();
}

ChatViewWidget::~ChatViewWidget()
{
    delete ui;
}

QListView* ChatViewWidget::getChatView() const
{
    return ui->chatView;
}

void ChatViewWidget::resizeEvent(QResizeEvent*)
{
    updateLayout(ui->chatView, true, true);
}

bool ChatViewWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->chatView->viewport()) {
        static bool mousePressed = false;
        if (event->type() == QEvent::MouseButtonPress) {
            const auto& mevt = static_cast<QMouseEvent*>(event);
            if (ui->chatView->geometry().contains(mevt->pos())) {
                mPressedPoint = mevt->pos();
                mousePressed = true;
            }
            return true;
        } else if (event->type() == QEvent::MouseMove) {
            const auto& mevt = static_cast<QMouseEvent*>(event);
            const int& d = mevt->y() - mPressedPoint.y();
            const int& sv = ui->chatView->verticalScrollBar()->value();
            // 视图滚动到了顶部
            if (sv == 0) {
                if (mLoader) {
                    mLoader->move(mLoader->x(), qMin(INDICATOR_PULLDOWN_LIMIT, d));
                } else if (mousePressed) {
                    mLoader = new LoadingIndicator(ui->chatView->viewport());
                    mLoader->show();
                }
            }
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            if (mLoader) {
                mLoader->start();
                DUMMY_DELAY_UI(100000)
                if (mLoader->y() > INDICATOR_PULLDOWN_LIMIT / 2 && mousePressed) {
                    const auto& index = mChatListModel->index(0);
                    mChatListModel->fetchMore();
                    ui->chatView->scrollTo(index);
                }
                mLoader->finish();
                mLoader = nullptr;
            }
            mousePressed = false;
            return true;
        }

        return false;
    }
    return QWidget::eventFilter(watched, event);
}

void ChatViewWidget::sendMessage(const QString& msg)
{
    mChatListModel->sendChat(AbstractChatListItem::Text, msg);
    ui->chatView->scrollToBottom();
}

void ChatViewWidget::autoDetermineScrollToBottom()
{
    const auto& sp = ui->chatView->spacing();
    QModelIndex index;

    // 从上面依次搜索找到第一个item
    for (int i = 2 * sp; i >= sp; i--) {
        index = ui->chatView->indexAt(QPoint(sp + 1, i));
        if (index.isValid()) {
            // 如果用户浏览倒数第4条之前的聊天记录就不要滚动到最低部
            if (index.row() < mChatListModel->rowCount() - 4) {
                return;
            }
            break;
        }
    }

    ui->chatView->scrollToBottom();
}
