#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStack>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChatList;
class ChatItem;
class ChatViewWidget;
class MessageList;
class MessageItem;
class IChatObject;
struct SChatItemPackage;
class QMenu;

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

private Q_SLOTS:

    /**
     * @brief 侦听所有失败消息的槽函数
     * @param msg 失败的返回消息
     */
    void onFailed(const QString& msg);

    /**
     * @brief 当消息列表弹出上下文菜单
     * @param index
     */
    //void onContextMenuInMessageItem(const QModelIndex& index);

    // 当消息激活时，一般为点击进行聊天
    void onMessageItemActived(const QModelIndex& index);

    void onReceived(const SChatItemPackage& package);

private:
    Ui::MainWindow* ui;

    /** 消息列表数据模型 */
    MessageList* mMessageListModel;

    /**
     * @brief 聊天视图表
     * @note 根据系统设置的最大聊天页面会动态地调整页面次序，并释放内存
     */
    QList<ChatViewWidget*> mChatPages;

    //QMenu* mMessageMenu;
};
#endif // MAINWINDOW_H
