#include "ChatViewWidget.h"
#include "ui_chatviewwidget.h"

#include <ChatItem.h>
#include <ChatList.h>
#include <LanObject.h>
#include <MessageDatabase.h>
#include <MessageManager.h>
#include <UI/MemberViewWidget.h>
#include <Utility.h>
#include <Widget/LoadingIndicator.h>

#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>

ChatViewWidget::ChatViewWidget(IChatObject* chatObj, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewWidget)
    , mLoader(nullptr)
{
    ui->setupUi(this);

    mChatListModel = new ChatList(this);

    STD_ICON_SETTER(ui->btnMenu, "menu");
    STD_ICON_SETTER(ui->btnInfo, "layout_info");

    // 取消文件传输
    if (chatObj->getRoleType() & IChatObject::MultiPerson) {
        ui->chatInputer->mBtnFile->setEnabled(false);
#ifndef QT_NO_TOOLTIP
        ui->chatInputer->mBtnFile->setToolTip(tr("暂不支持局域网发送文件"));
#endif
    }

    // 标题
    ui->lbTitle->setText(chatObj->getNickName());

    // 视图
    ui->chatView->setItemDelegate(new ChatItemDelegate(this));
    ui->chatView->setModel(mChatListModel);
    ui->chatView->viewport()->installEventFilter(this);

    connect(ui->chatInputer, &ChatInputBox::expand, this, &ChatViewWidget::autoDetermineScrollToBottom);
    connect(ui->chatInputer, &ChatInputBox::send, this, &ChatViewWidget::sendChat);
    connect(ui->btnMenu, &QPushButton::clicked, this, &ChatViewWidget::builChatViewWidgetMenu);
    connect(ui->btnInfo, &QPushButton::clicked, this, &ChatViewWidget::openChatObjectInfo);

    // 调整显示层
    ui->chatView->lower();
    lower();

    mChatListModel->initLoad(chatObj);
    ui->chatView->scrollToBottom();
}

ChatViewWidget::~ChatViewWidget()
{
    delete ui->btnInfo;
    ui->btnInfo = nullptr;

    delete ui;
}

QListView* ChatViewWidget::getChatView(void) const
{
    return ui->chatView;
}

void ChatViewWidget::sendChat(int type, const QVariant& data) noexcept
{
    MessageManager::Instance()->sendMessage(mChatListModel, type, data);
}

void ChatViewWidget::resizeEvent(QResizeEvent*)
{
    UpdateLayout(ui->chatView, true, true);
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
        }
        if (event->type() == QEvent::MouseMove) {
            const auto& mevt = static_cast<QMouseEvent*>(event);
            const int& d = mevt->y() - mPressedPoint.y();
            const int& sv = ui->chatView->verticalScrollBar()->value();
            // 视图滚动到了顶部
            if (sv == 0) {
                if (mLoader) {
                    mLoader->move(mLoader->x(), qMin(INDICATOR_PULLDOWN_LIMIT, d) - INDICATOR_SIZE);
                } else if (mousePressed) {
                    mLoader = new LoadingIndicator(ui->chatView->viewport());
                    mLoader->show();
                }
            }
            return true;
        }
        if (event->type() == QEvent::MouseButtonRelease) {
            if (mLoader && !mLoader->isLoading()) {
                mLoader->start();
                if (mLoader->y() > INDICATOR_PULLDOWN_LIMIT / 2 && mousePressed) {
                    const auto& index = mChatListModel->index(0);
                    DUMMY_DELAY_UI(100000)
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

void ChatViewWidget::autoDetermineScrollToBottom()
{
    const auto& sp = ui->chatView->spacing();
    QModelIndex index;

    // 从上面依次搜索找到第一个item
    for (int i = sp + 1; i > 0; --i) {
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

void ChatViewWidget::builChatViewWidgetMenu()
{
    QMenu menu;
    QAction* refreshAction = new QAction(ICON("refresh"), tr("刷新"), &menu);
    QAction* clearAction = new QAction(ICON("trash"), tr("清屏"), &menu);
    QAction* removeAction = new QAction(ICON("remove"), tr("删除聊天记录"), &menu);
    menu.addAction(refreshAction);
    menu.addAction(clearAction);
    menu.addSeparator();
    menu.addAction(removeAction);

    connect(refreshAction, &QAction::triggered, [this] { mChatListModel->fetchMore(); });
    connect(clearAction, &QAction::triggered, mChatListModel, &ChatList::clear);
    connect(removeAction, &QAction::triggered, this, &ChatViewWidget::clearRecord);

    menu.exec(cursor().pos());
}

void ChatViewWidget::clearRecord()
{
    // TODO: 弹出对话框
    mChatListModel->clear();

    MessageDatabase::Instance()->clearChatRecord(mChatListModel->getChatObject()->getUuid());
}

void ChatViewWidget::openChatObjectInfo()
{
    switch (mChatListModel->getChatObject()->getRoleType()) {
    case IChatObject::LAN: {
        ui->btnInfo->setEnabled(false);

        MemberViewWidget* mvw = new MemberViewWidget(static_cast<LanObject*>(mChatListModel->getChatObject()), this);

        // 尽早显示
        mvw->show();

        QRect r(ui->btnInfo->x() - 300, ui->btnInfo->geometry().bottom(), 0, 0);
        r.setWidth(360);
        r.setHeight(qMax(height() - r.top(), 200));
        mvw->setGeometry(r);

        connect(mvw, &MemberViewWidget::destroyed, [this] {
            if (ui->btnInfo) {
                ui->btnInfo->setEnabled(true);
            }
        });
    } break;
    default:
        break;
    }
}
