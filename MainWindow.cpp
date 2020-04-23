#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <AppSettings.h>
#include <ChatList.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MessageManager.h>
#include <UI/ChatViewWidget.h>
#include <User.h>
#include <Utility.h>
#include <Widget/PromptWidget.h>

// 聊天控件
#include <TextChatItem.h>

#include <QListView>
#include <QMenu>
#include <QScrollBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1080, 640);

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

    connect(ui->navigation, &Navigation::navigated, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->messageList, &QListView::clicked, this, &MainWindow::onMessageItemActived);
    connect(mMessageListModel, &MessageList::failed, this, &MainWindow::onFailed);
    connect(MessageManager::Instance().data(), &MessageManager::received, this, &MainWindow::onReceived, Qt::QueuedConnection);

    // 当聊天视图滚动到最底端时，又展开输入框后，会使得视图布局不更新，导致无法看到最后几条消息。
    // connect(ui->chatInputer, &ChatInputBox::onFoldup, [=] {
    //     updateLayout(this);
    //     const auto vsb = ui->chatView->verticalScrollBar();
    //     if (vsb->value() > vsb->maximum() - ui->chatInputer->height()) {
    //         ui->chatView->scrollToBottom();
    //     }
    // });

    // 注册聊天控件，TODO: 添加更多
    MessageManager::RegisterChatItemClass<TextChatItem>();

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
    mChatPages.append(cvw);

    cvw->getChatListModel()->initLoad(chatObj);
    cvw->getChatView()->scrollToBottom();

    return cvw;
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
    bool isChatPageExists = false;
    ui->lbEmptyChat->setVisible(false);
    const auto& msgItem = index.data(MessageItemDelegate::MessageItemRole).value<MessageItem*>();
    for (int i = 0; i < mChatPages.length(); i++) {
        mChatPages.at(i)->setVisible(false);
        if (mChatPages.at(i)->getChatListModel()->getChatObject() == msgItem->getChatObject()) {
            mChatPages.at(i)->setVisible(true);

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
     */

    int i = 0;
    const int len = mMessageListModel->rowCount();
    for (; i < len; i++) {
        const auto& msgItem = mMessageListModel->getMessage(i);
        if (package.RoleType & IChatObject::MultiPerson) {
            // 如果数据中的 局域网IP地址 和该聊天对象匹配就接收数据
            if (package.HostAddress == msgItem->getChatObject()->getHostAddress()) {
                break;
            }
        } else if (package.UserChatData.Uuid == msgItem->getChatObject()->getUuid()) {
            break;
        }
    }

    // 构建消息
    ChatItem* item = MessageManager::BuildChatItem(package.ChatType, package.UserChatData, package.Data);
    if (!item) {
        // 无效消息
        item = new TextChatItem;
        item->setSendState(TextChatItem::Failed);
        item->setData(QStringLiteral("[消息失效]"));
        return;
    }

    // I.i
    if (i == len) {
        if (!package.UserChatData.isEmpty()) {
            // I.ii.a
            const auto& chatObj = User::Instance()->getChatObjectByUuid(package.UserChatData.Uuid);
            if (chatObj) {
                MessageDatabase::Instance()->newMessageItem(mMessageListModel, chatObj);
                MessageDatabase::Instance()->saveAChatRecord(item, chatObj->getUuid());
            }
        }
    } else {
        // I.ii
        const auto& msgItem = mMessageListModel->getMessage(i);
        if (mMessageListModel->getRow(msgItem) == ui->messageList->currentIndex().row()) {
            const auto& list = mChatPages.first();
            // 视图中是否可以接收消息
            // if (!list->canSend(item)) {
            //      SAFE_DELETE(item);
            //      return;
            // }
            list->getChatListModel()->appendItem(item);
        } else {
            // If I.ii
            msgItem->setReadFlag(false);
            msgItem->setUnreadMsgCount(msgItem->getUnreadMsgCount() + 1);
        }
        msgItem->setMessage(package.Data.toString());
        msgItem->setTime(package.Time);
        mMessageListModel->ariseMessage(msgItem);

        // 保存到数据库
        MessageDatabase::Instance()->saveAChatRecord(item, msgItem->getChatObject()->getUuid());
    }
}
