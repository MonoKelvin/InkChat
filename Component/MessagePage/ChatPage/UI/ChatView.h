#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QListWidget>

class IChatItem;
class QLabel;
class QPushButton;
class User;
class ChatInputBox;

class ChatView : public QListWidget {
    Q_OBJECT
public:
    explicit ChatView(QWidget* parent = nullptr);
    explicit ChatView(QSharedPointer<User> user, QWidget* parent);

    /**
     * @brief 添加一条聊天控件
     * @param chat 聊天控件
     * @param isScrollToBottom 添加完后是否滚动到最底部
     */
    void addChatWidget(IChatItem* chat, bool isScrollToBottom = true);

    void setHeaderTitle(const QString& title);

public slots:
    /**
     * @brief 清空聊天消息内容，只影响界面，不会更新数据库
     */
    void clearChats();

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
    QPushButton* mBtnTool;

    /** 聊天视图 */
    QListView* mChatView;
};

#endif // CHATVIEW_H
