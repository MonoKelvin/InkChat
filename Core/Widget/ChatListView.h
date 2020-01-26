#ifndef CHATLISTVIEW_H
#define CHATLISTVIEW_H

#include <QListWidget>

class IChatWidget;
class QLabel;
class QPushButton;
class User;

class ChatListView : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatListView(QWidget *parent = nullptr);
    explicit ChatListView(QSharedPointer<User> user, QWidget *parent);

    /**
     * @brief 添加一条聊天控件
     * @param chat 聊天控件
     * @param isScrollToBottom 添加完后是否滚动到最底部
     */
    void addChatWidget(IChatWidget *chat, bool isScrollToBottom = true);

protected:
    void resizeEvent(QResizeEvent *e) override;

private:
    /**
     * @brief 更新视图
     */
    void updateViewport(void);

private:
    /** 顶部标题文字 */
    QLabel *mHeaderTitle;

    /** 顶部工具按钮 */
    QPushButton *mBtnTool;
};

#endif // CHATLISTVIEW_H
