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
    connect(MessageManager::Instance(), &MessageManager::received, this, &MainWindow::onReceived);

    // 当聊天视图滚动到最底端时，又展开输入框后，会使得视图布局不更新，导致无法看到最后几条消息。
    // connect(ui->chatInputer, &ChatInputBox::onFoldup, [=] {
    //     updateLayout(this);
    //     const auto vsb = ui->chatView->verticalScrollBar();
    //     if (vsb->value() > vsb->maximum() - ui->chatInputer->height()) {
    //         ui->chatView->scrollToBottom();
    //     }
    // });

    // 注册聊天控件，TODO: 添加更多
    REGISTER_CHATITEM(TextChatItem);

    // 加载缓存消息，必须放在最后，因为加载数据需要关联各种信号并先注册号聊天类等。
    mMessageListModel->load();
}

MainWindow::~MainWindow()
{
    mChatPages.clear();

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
    int i = 0;
    for (; i < mChatPages.length(); i++) {
        const auto& model = mChatPages.at(i)->getChatListModel();
        bool receive = false;
        if (package.RoleType & IChatObject::MultiPerson) {
            // 如果数据中的 局域网IP地址 和该聊天对象匹配就接收数据
            if (package.HostAddress == model->getChatObject()->getHostAddress()) {
                receive = true;
            }
        } else {
            // 如果数据中的单用户uuid和该聊天对象的用户uuid匹配就接收数据
            if (package.UserChatData.Uuid == model->getChatObject()->getUuid()) {
                receive = true;
            }
        }

        if (!receive) {
            continue;
        }

        // 推送到聊天视图中
        ChatItem* item = MessageManager::BuildChatItem(package.ChatType, package.UserChatData, package.Data);

        // 不接收出错消息
        if (!item) {
            return;
        }

        model->appendItem(item);
        MessageManager::Instance()->saveAChatRecord(model, item);

        // 一般只在一个聊天视图中接收数据
        break;
    }

    // 如果有用户发来消息，但消息列表中没有，就添加一项
    if (i == mChatPages.length() && !package.UserChatData.Uuid.isEmpty()) {
        // 是否有缓存的消息对象
        //mMessageListModel;

        const auto& chatObj = User::Instance()->getChatObjectByUuid(package.UserChatData.Uuid);

        createChatViewWidget(chatObj);
    }
}
