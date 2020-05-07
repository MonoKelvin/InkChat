#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <AppSettings.h>
#include <ChatList.h>
#include <LanObject.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MessageManager.h>
#include <TcpClient.h>
#include <UI/ChatViewWidget.h>
#include <User.h>
#include <Widget/PromptWidget.h>

// 聊天控件
#include <FileChatItem.h>
#include <NotificationItem.h>
#include <TextChatItem.h>

#include <QMenu>
#include <QMouseEvent>

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
    connect(ui->settingsPage, &SettingsPage::failed, this, &MainWindow::onFailed);
    connect(MessageManager::Instance().data(), &MessageManager::received, this, &MainWindow::onReceived, Qt::QueuedConnection);
    connect(MessageManager::Instance().data(), &MessageManager::failed, this, &MainWindow::onFailed);
    connect(MessageManager::Instance().data(), &MessageManager::chat, this, &MainWindow::onChat);

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
    ChatViewWidget* cvw = new ChatViewWidget(chatObj, this);
    ui->messagePage->layout()->addWidget(cvw);

    // 缓存
    int max = AppSettings::Value(QStringLiteral("App/maxChatPageCount"), 3).toInt();
    max = max > 1 ? (max < 32 ? max : 32) : 1;

    if (mChatPages.length() > max) {
        mChatPages.last()->deleteLater();
        mChatPages.removeLast();
    }
    mChatPages.prepend(cvw);

    // 多人聊天就发送用户进入消息，并请求所有其他用户的信息
    if (cvw->getChatListModel()->getChatObject()->getRoleType() & IChatObject::MultiPerson) {
        MessageManager::Instance()->sendMessage(chatObj, ChatItem::UserJoin);
        MessageManager::Instance()->sendMessage(chatObj, ChatItem::RequestUserInfo);
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

    // 该项是否被选中
    if (ui->messageList->selectionModel()->isSelected(mMessageListModel->itemIndex(item))) {
        QAction* deselectAction = new QAction(tr("取消选择"), &menu);
        connect(deselectAction, &QAction::triggered, this, &MainWindow::clearMessageItemSelection);
        menu.addAction(deselectAction);
        readAction->setEnabled(false);
    } else {
        connect(readAction, &QAction::triggered, [this, item] {
            item->setReadFlag(!item->getReadFlag());
            ui->messageList->update(mMessageListModel->itemIndex(item));
        });
    }

    if (item->getChatObject()->getRoleType() == IChatObject::LAN) {
        QAction* avatarAction = new QAction(tr("选择头像"), &menu);
        connect(avatarAction, &QAction::triggered, item->getChatObject(), &IChatObject::selectAvatarFile);
        menu.addAction(avatarAction);
    }

    connect(topAction, &QAction::triggered, [this, item] {
        item->setIsTop(!item->isTop());
        if (item->isTop()) {
            mMessageListModel->promoteMessage(item);
        } else {
            mMessageListModel->loweredMessage(item);
        }
        ui->messageList->update(ui->messageList->rect());
    });
    connect(removeAction, &QAction::triggered, [this, item] {
        mMessageListModel->removeMessage(item);
        ui->messageList->update(ui->messageList->rect());

        ChatViewWidget* cvw = findChatViewByMessageItem(item);
        if (cvw) {
            mChatPages.removeOne(cvw);
            delete cvw;
            cvw = nullptr;
            onMessageItemActived(QModelIndex());
        }
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

ChatViewWidget* MainWindow::findChatViewByMessageItem(MessageItem* item) const
{
    for (int i = 0; i < mChatPages.size(); ++i) {
        if (mChatPages.at(i)->getChatListModel()->getChatObject() == item->getChatObject()) {
            return mChatPages.at(i);
        }
    }

    return nullptr;
}

int MainWindow::getMessageIndexByPackage(const SChatItemPackage& package)
{
    const int len = mMessageListModel->rowCount();
    if (package.RoleType & IChatObject::MultiPerson || package.UserChatData.Uuid == User::Instance()->getUuid()) {
        for (int i = 0; i < len; ++i) {
            if (package.IPAddress == mMessageListModel->getMessage(i)->getChatObject()->getHostAddress()) {
                return i;
            }
        }
    } else {
        for (int i = 0; i < len; ++i) {
            if (package.UserChatData.Uuid == mMessageListModel->getMessage(i)->getChatObject()->getUuid()) {
                return i;
            }
        }
    }

    return -1;
}

void MainWindow::onFailed(const QString& msg) noexcept
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

    // 更新消息内容
    const auto& msgItem = index.data(MessageItemDelegate::MessageItemRole).value<MessageItem*>();
    msgItem->setReadFlag(true);
    msgItem->setUnreadMsgCount(0);
    ui->messageList->update(index);

    // 找到是否有页面缓存
    bool isChatPageCached = false;
    for (int i = 0; i < mChatPages.length(); i++) {
        mChatPages.at(i)->setVisible(false);
        if (mChatPages.at(i)->getChatListModel()->getChatObject() == msgItem->getChatObject()) {
            mChatPages.at(i)->setVisible(true);
            isChatPageCached = true;

            // 把最近使用的页面放置在最前
            mChatPages.move(i, 0);
        }
    }

    // 如果没有缓存就新建一个聊天视图
    if (!isChatPageCached) {
        createChatViewWidget(msgItem->getChatObject());
    }
}

void MainWindow::onChat(IChatObject* chatObj)
{
    MessageItem* item = MessageDatabase::Instance()->loadMessageItem(mMessageListModel, chatObj);

    if (item) {
        onMessageItemActived(mMessageListModel->itemIndex(item));
        ui->messageList->setCurrentIndex(mMessageListModel->itemIndex(item));
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

    // I
    const int i = getMessageIndexByPackage(package); // 返回-1表示不存在

    // 如果是用户的行为状态
    if (package.ChatType & AbstractChatListItem::UserBehavior && i != -1) {
        const auto& chatObj = mMessageListModel->getMessage(i)->getChatObject();

        // 如果多人聊天的（局域网）地址匹配
        if (chatObj->getHostAddress() == package.IPAddress) {
            switch (package.ChatType) {
            case ChatItem::UserJoin:
            case ChatItem::UserLeft: {
                const auto& chatView = findChatViewByMessageItem(mMessageListModel->getMessage(i));
                if (chatView) {
                    const auto& notice = new NotificationItem(MessageManager::BehaviorString(package.ChatType, package.UserChatData.Name));
                    chatView->getChatListModel()->appendItem(notice);
                    chatView->autoDetermineScrollToBottom();
                }
            }
                // 不要加 break
            case ChatItem::ReplyUserInfo: // 我的请求得到回复，则更新列表
                if (package.RoleType & IChatObject::LAN) {
                    const auto& lan = static_cast<LanObject*>(chatObj);
                    lan->setMemberBehavior(package.ChatType, { package.UserChatData.Uuid, package.UserChatData.Name, package.HostAddress });
                }
                break;
                // 有用户请求要获取我的数据
            case ChatItem::RequestUserInfo:
                MessageManager::Instance()->sendMessage(chatObj, ChatItem::ReplyUserInfo);
                break;
            }

            return;
        }
    }

    // 消息概要
    const auto& brief = dealWithMessageBrief(package.ChatType, package.UserChatData.Data.toString());

    // II.i
    if (package.UserChatData.Uuid == User::Instance()->getUuid()) {
        const auto& curIndex = ui->messageList->currentIndex();
        const auto& msgItem = mMessageListModel->getMessage(curIndex.row());
        msgItem->setTime(QDateTime::currentDateTime());
        msgItem->setMessage(brief);
        ui->messageList->update(curIndex);
        return;
    }

    // 构建消息
    ChatItem* item = MessageManager::BuildChatItem(package.ChatType, package.UserChatData);
    if (!item) {
        return;
    }

    // 如果收到TCP类型的消息就新建TCP客户端来处理
    if (package.ChatType & ChatItem::TCP_Type) {
        if (package.IPAddress == User::Instance()->getHostAddress()) {
            TcpClient* client = new TcpClient(this);
            client->connectToHost(package.IPAddress, item);
        }
    }

    // I.i
    if (i == -1) {
        // I.i.a & I.i.b
        const auto& chatObj = User::Instance()->getChatObjectByUuid(package.UserChatData.Uuid, true);
        if (chatObj) {
            chatObj->setNickName(package.UserChatData.Name);
            chatObj->setHostAddress(package.IPAddress);

            // 更新
            const auto& msgItem = MessageDatabase::Instance()->loadMessageItem(mMessageListModel, chatObj);
            msgItem->setReadFlag(false);
            msgItem->setUnreadMsgCount(1);
            msgItem->setMessage(brief);
            msgItem->setTime(QDateTime::currentDateTime());
            mMessageListModel->promoteMessage(msgItem);
            ui->messageList->update(ui->messageList->rect());

            MessageDatabase::Instance()->saveAChatRecord(item, chatObj->getUuid());
            chatObj->updateLocalData();
        }
    } else {
        const auto& msgItem = mMessageListModel->getMessage(i);
        const int tarRow = mMessageListModel->getRow(msgItem);

        // I.ii.a
        if (ui->messageList->selectionModel()->isSelected(mMessageListModel->index(tarRow))) {
            mChatPages.first()->getChatListModel()->appendItem(item);
            mChatPages.first()->autoDetermineScrollToBottom();
        } else {
            // I.ii.b
            msgItem->setReadFlag(false);
            msgItem->setUnreadMsgCount(msgItem->getUnreadMsgCount() + 1);

            // 如果有缓存的聊天页面则添加到界面
            const auto& cv = findChatViewByMessageItem(msgItem);
            if (cv) {
                cv->getChatListModel()->appendItem(item);
                cv->autoDetermineScrollToBottom();
            }
        }

        msgItem->setMessage(brief);
        msgItem->setTime(QDateTime::currentDateTime());
        mMessageListModel->promoteMessage(msgItem);
        ui->messageList->update(ui->messageList->rect());

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
        return message.left(32).remove(QLatin1String("\n"));
    case AbstractChatListItem::UserJoin:
        return QStringLiteral("[新用户加入]");
    case AbstractChatListItem::UserLeft:
        return QStringLiteral("[有用户离开]");
    }

    return message;
}
