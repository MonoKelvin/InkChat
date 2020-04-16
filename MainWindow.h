#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChatView;
class MessageList;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // void dynamicDestroyForChatView();

private:
    Ui::MainWindow* ui;

    /** 消息列表数据模型 */
    MessageList* mMessageListModel;

    /**
     * @brief 聊天视图的集合
     */
    QList<QPointer<ChatView>> mChatViewsList;
};
#endif // MAINWINDOW_H
