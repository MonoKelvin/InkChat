#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <AppSettings.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MessageManager.h>
#include <TcpClient.h>
#include <UI/ChatViewWidget.h>
#include <User.h>
#include <Utility.h>
#include <Widget/PromptWidget.h>

// 聊天控件
#include <FileChatItem.h>
#include <NotificationItem.h>
#include <TextChatItem.h>

#include <QListView>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1080, 640);
    setWindowTitle(QStringLiteral("InkChat-v%1 - %2").arg(CHAT_VERSION_STR).arg(User::Instance()->getNickName()));

    // 注册聊天控件，TODO: 添加更多
    MessageManager::RegisterChatItemClass<TextChatItem>();
    MessageManager::RegisterChatItemClass<FileChatItem>();
    //MessageManager::RegisterChatItemClass<NotificationItem>();

    // 注册元对象，这样这些自定义数据就可以在信号和槽中作为参数使用
    qRegisterMetaType<SChatItemPackage>("SChatItemPackage");
    //qRegisterMetaType<SUserBaseData>("SUserBaseData");

    //ui->chatView->lower();
    ui->messageList->raise();
    ui->msgTitle->raise();
    ui->navigation->raise();
    // attachShadowEffect(ui->messageList, 1.0);
    // attachShadowEffect(ui->navigation, 1.0);

    // 导航栏
    ui->navigation->addNavButton(QIcon::fromTheme("message"));
    ui->navigation->addNavButton(QIcon::fromTheme("person"));
    ui->navigation->setUser(User::Instance());
    ui->navigation->navigate(0);

    // 消息列表
    mMessageListModel = new MessageList(this);
    ui->messageList->setItemDelegate(new MessageItemDelegate(this)); // 设置代理
    ui->messageList->setModel(mMessageListModel); // 设置模型
    ui->messageList->viewport()->installEventFilter(this);

    connect(ui->navigation, &Navigation::navigated, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->messageList, &QListView::clicked, this, &MainWindow::onMessageItemActived);
    connect(mMessageListModel, &MessageList::failed, this, &MainWindow::onFailed);
    connect(MessageManager::Instance().data(), &MessageManager::received, this, &MainWindow::onReceived, Qt::QueuedConnection);
    connect(MessageManager::Instance().data(), &MessageManager::failed, this, &MainWindow::onFailed);

    // 加载缓存消息，必须放在最后，因为加载数据需要关联各种信号并先注册号聊天类等。
    mMessageListModel->load();
}

MainWindow::~MainWindow()
{
    mChatPages.clear();
    delete User::Instance();
    User::Instance() = nullptr;

    delete ui;
}

ChatViewWidget* MainWindow::createChatViewWidget(IChatObject* chatObj)
{
    ChatViewWidget* cvw = new ChatViewWidget(this);
    ui->messagePage->layout()->addWidget(cvw);

    // 缓存
    int max = AppSettings::Value(QStringLiteral("App/maxChatPageCount"), 3).toInt();
    max = max > 1 ? (max < 32 ? max : 32) : 1;

    if (mChatPages.length() > max) {
        mChatPages.last()->deleteLater();
        mChatPages.removeLast();
    }
    mChatPages.prepend(cvw);

    cvw->getChatListModel()->initLoad(chatObj);
    cvw->getChatView()->scrollToBottom();
    cvw->setTitle(chatObj->getNickName());

    // 多人聊天就发送用户进入消息
    if (cvw->getChatListModel()->getChatObject()->getRoleType() & IChatObject::MultiPerson) {
        MessageManager::Instance()->sendUserBehavior(chatObj->getHostAddress(), ChatItem::UserJoin);
    }

    return cvw;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->messageList->viewport()) {
        if (event->type() == QEvent::ContextMenu) {
            const auto& mevt = static_cast<QMouseEvent*>(event);
            const auto& index = ui->messageList->indexAt(mevt->pos());
            buildMessageItemMenu(index.data(MessageItemDelegate::MessageItemRole).value<MessageItem*>());
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::buildMessageItemMenu(MessageItem* item)
{
    if (!item) {
        return;
    }

    QMenu menu;
    QAction* topAction = new QAction(&menu);
    QAction* readAction = new QAction(&menu);
    QAction* removeAction = new QAction(QIcon::fromTheme(QStringLiteral("trash_black")), tr("从列表中移除"), &menu);
    menu.addAction(topAction);
    menu.addAction(readAction);
    menu.addAction(removeAction);

    if (item->getReadFlag()) {
        readAction->setIcon(QIcon::fromTheme(QStringLiteral("unread")));
        readAction->setText(tr("设为未读"));
    } else {
        readAction->setIcon(QIcon::fromTheme(QStringLiteral("read")));
        readAction->setText(tr("设为已读"));
    }

    if (ui->messageList->selectionModel()->isSelected(mMessageListModel->itemIndex(item))) {
        QAction* deselectAction = new QAction(tr("取消选择"), &menu);
        connect(deselectAction, &QAction::triggered, this, &MainWindow::clearMessageItemSelection);
        menu.addAction(deselectAction);
        //readAction->setEnabled(false);
    }

    if (item->getChatObject()->getRoleType() == IChatObject::LAN) {
        QAction* avatarAction = new QAction(tr("选择头像"), &menu);
        connect(avatarAction, &QAction::triggered, item->getChatObject(), &IChatObject::selectAvatarFile);
        menu.addAction(avatarAction);
    }

    connect(topAction, &QAction::triggered, [this, item] {
        item->setIsTop(!item->isTop());
        ui->messageList->update(ui->messageList->rect());
    });
    connect(readAction, &QAction::triggered, [this, item] {
        item->setReadFlag(!item->getReadFlag());
        ui->messageList->update(ui->messageList->rect());
    });
    connect(removeAction, &QAction::triggered, [this, item] {
        mMessageListModel->removeMessage(item);
        ui->messageList->update(ui->messageList->rect());
    });

    if (item->isTop()) {
        topAction->setIcon(QIcon::fromTheme(QStringLiteral("untop")));
        topAction->setText(tr("取消置顶"));
    } else {
        topAction->setIcon(QIcon::fromTheme(QStringLiteral("top")));
        topAction->setText(tr("置顶"));
    }

    menu.exec(cursor().pos());
}

void MainWindow::onFailed(const QString& msg)
{
    new PromptWidget(msg, this);
}

void MainWindow::onMessageItemActived(const QModelIndex& index)
{
    if (!index.isValid()) {
        ui->lbEmptyChat->setVisible(true);
        return;
    }

    // 关闭提示和其他聊天视图，如果有缓存就直接显示缓存的内容
    ui->lbEmptyChat->setVisible(false);

    bool isChatPageExists = false;
    const auto& msgItem = index.data(MessageItemDelegate::MessageItemRole).value<MessageItem*>();
    for (int i = 0; i < mChatPages.length(); i++) {
        mChatPages.at(i)->setVisible(false);
        if (mChatPages.at(i)->getChatListModel()->getChatObject() == msgItem->getChatObject()) {
            mChatPages.at(i)->setVisible(true);
            isChatPageExists = true;

            // 把最近使用的页面放置在最前
            mChatPages.move(i, 0);
        }
    }

    // 如果没有缓存就新建一个聊天视图
    if (!isChatPageExists) {
        createChatViewWidget(msgItem->getChatObject());
    }
}

void MainWindow::onReceived(const SChatItemPackage& package)
{
    /*
     * 接收到消息的算法步骤
     * I. 判断消息列表中该联系人是否存在
     *   i.不存在
     *     a.联系人有缓存：接收该消息并保存到数据库，加载缓存到消息列表指定位置，不打开聊天视图
     *     b.联系人无缓存：先新建缓存，人后重复 ii.a 操作
     *   ii.存在（最后保存到数据库）
     *     a.被选中：直接发送到对应的聊天视图、更新其在消息列表的位置、最近消息、时间等信息
     *     b.未被选中：更新其在消息列表的位置、未读消息数、最近消息、时间等信息，不打开聊天视图
     * II. 如果接收到自己发送的消息
     *   i.一定打开了聊天视图、选中了某个消息项：直接更新那个消息项
     */

    // 如果是用户的行为状态
    if (package.ChatType & AbstractChatListItem::UserBehavior) {
        if (!mChatPages.isEmpty() && mChatPages.first()->getChatListModel()->getChatObject()->getHostAddress() == package.HostAddress) {
            const auto& notice = new NotificationItem(MessageManager::GetUserBehavior(package.ChatType, package.UserChatData.Name));
            mChatPages.first()->getChatListModel()->appendItem(notice);
            mChatPages.first()->autoDetermineScrollToBottom();
        }
        return;
    }

    // 消息概要
    const auto& brief = dealWithMessageBrief(package.RoleType, package.UserChatData.Data.toString());

    // II.i
    if (package.UserChatData.Uuid == User::Instance()->getUuid()) {
        const auto& curIndex = ui->messageList->currentIndex();
        const auto& msgItem = mMessageListModel->getMessage(curIndex.row());
        msgItem->setTime(QDateTime::currentDateTime());
        msgItem->setMessage(brief);
        ui->messageList->update(curIndex);
        return;
    }

    // I
    int i = 0;
    const int len = mMessageListModel->rowCount();
    for (; i < len; i++) {
        if (package.RoleType & IChatObject::MultiPerson) {
            if (package.HostAddress == mMessageListModel->getMessage(i)->getChatObject()->getHostAddress()) {
                break;
            }
        } else if (package.UserChatData.Uuid == mMessageListModel->getMessage(i)->getChatObject()->getUuid()) {
            break;
        }
    }

    // 构建消息
    ChatItem* item = MessageManager::BuildChatItem(package.ChatType, package.UserChatData);
    if (item) {
        item->setParent(this);
    } else {
        return;
    }

    switch (package.ChatType) {
    case AbstractChatListItem::File:
        if (package.HostAddress == User::Instance()->getHostAddress()) {
            TcpClient* client = new TcpClient(this);
            client->connectToHost(package.HostAddress, item);
        }
        break;
    case AbstractChatListItem::Text:
        break;
    }

    // I.i
    if (i == len) {
        // I.ii.a
        const auto& chatObj = User::Instance()->getChatObjectByUuid(package.UserChatData.Uuid);
        if (chatObj) {
            MessageDatabase::Instance()->newMessageItem(mMessageListModel, chatObj);
            MessageDatabase::Instance()->saveAChatRecord(item, chatObj->getUuid());
        }
    } else {
        const auto& msgItem = mMessageListModel->getMessage(i);
        const int& tarRow = mMessageListModel->getRow(msgItem);

        // I.ii.a
        if (ui->messageList->selectionModel()->isSelected(mMessageListModel->index(tarRow))) {
            mChatPages.first()->getChatListModel()->appendItem(item);
            mChatPages.first()->autoDetermineScrollToBottom();
        } else {
            // I.ii.b
            msgItem->setReadFlag(false);
            msgItem->setUnreadMsgCount(msgItem->getUnreadMsgCount() + 1);
        }

        msgItem->setMessage(brief);
        msgItem->setTime(QDateTime::currentDateTime());
        mMessageListModel->ariseMessage(msgItem);
        ui->messageList->update(ui->messageList->rect());

        // 保存到数据库
        MessageDatabase::Instance()->saveAChatRecord(item, msgItem->getChatObject()->getUuid());
    }
}

void MainWindow::clearMessageItemSelection()
{
    ui->messageList->clearSelection();
    ui->lbEmptyChat->setVisible(true);
    for (int i = 0; i < mChatPages.length(); i++) {
        mChatPages.at(i)->setVisible(false);
    }
}

const QString MainWindow::dealWithMessageBrief(int type, const QString& message) noexcept
{
    switch (type) {
    case AbstractChatListItem::File:
        return (QStringLiteral("[文件]") + GetFileNameFromPath(message)).left(32);
    case AbstractChatListItem::Text:
        return message.left(32);
    case AbstractChatListItem::UserJoin:
        return QStringLiteral("[新用户加入]");
    case AbstractChatListItem::UserLeft:
        return QStringLiteral("[有用户离开]");
    }

    return message;
}
