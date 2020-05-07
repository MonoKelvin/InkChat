#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChatViewWidget;
class MessageList;
class MessageItem;
class IChatObject;
struct SChatItemPackage;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    /**
     * @brief 新建一个聊天视图
     * @note 新建好的视图会自动加载历史数据
     */
    ChatViewWidget* createChatViewWidget(IChatObject* chatObj);

    bool eventFilter(QObject* watched, QEvent* event) override;

    void buildMessageItemMenu(MessageItem* item);

    ChatViewWidget* findChatViewByMessageItem(MessageItem* item) const;

    /**
     * @brief 通过消息包找到要处理的消息项
     * @param package 消息包
     * @return 如果找到则返回消息项行数，否则返回-1
     */
    int getMessageIndexByPackage(const SChatItemPackage& package);

private Q_SLOTS:

    /**
     * @brief 侦听所有失败消息的槽函数
     * @param msg 失败的返回消息
     */
    void onFailed(const QString& msg) noexcept;

    /**
     * @brief 当消息列表弹出上下文菜单
     * @param index
     */
    //void onContextMenuInMessageItem(const QModelIndex& index);

    // 当消息激活时，一般为点击进行聊天
    void onMessageItemActived(const QModelIndex& index);

    // 当需要聊天时，一般直接打开消息项
    void onChat(IChatObject* chatObj);

    void onReceived(const SChatItemPackage& package);

    void clearMessageItemSelection();

    const QString dealWithMessageBrief(int type, const QString& message) noexcept;

private:
    Ui::MainWindow* ui;

    /** 消息列表数据模型 */
    MessageList* mMessageListModel;

    /**
     * @brief 聊天视图表
     * @note 根据系统设置的最大聊天页面会动态地调整页面次序，并释放内存
     */
    QList<ChatViewWidget*> mChatPages;
};
#endif // MAINWINDOW_H
