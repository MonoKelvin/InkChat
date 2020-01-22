#ifndef CHATLISTVIEW_H
#define CHATLISTVIEW_H

#include <QListWidget>

class IChatWidget;

class ChatListView : public QListWidget
{
    Q_OBJECT
public:
    ChatListView(QWidget *parent = nullptr);

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
     * @brief 更新可见视图内的所有item
     */
    void updateViewportItems(void);
};

#endif // CHATLISTVIEW_H
