#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChatList;
class MessageList;
class QMenu;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private Q_SLOTS:
    // void dynamicDestroyForChatView();

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

private:
    Ui::MainWindow* ui;

    /** 消息列表数据模型 */
    MessageList* mMessageListModel;

    /**
     * @brief 聊天视图的集合
     */
    QList<QPointer<ChatList>> mChatViewsList;

    //QMenu* mMessageMenu;
};
#endif // MAINWINDOW_H
