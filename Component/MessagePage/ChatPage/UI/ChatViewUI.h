#ifndef CHATVIEWUI_H
#define CHATVIEWUI_H

#include <ChatView.h>

#include <QListView>

class IChatItem;
class QLabel;
class QPushButton;
class User;
class ChatInputBox;

class ChatViewUI : public QListView {
    Q_OBJECT
public:
    explicit ChatViewUI(QWidget* parent = nullptr);

    /**
     * @brief 添加一条聊天控件
     * @param chat 聊天控件
     * @param isScrollToBottom 添加完后是否滚动到最底部
     */
    void addChatWidget(IChatItem* chat, bool isScrollToBottom = true);

    void setHeaderTitle(const QString& title);

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    /**
     * @brief 更新视图
     */
    void updateViewport(void);

private:
    /** 顶部标题文字 */
    QLabel* mHeaderTitle;

    /** 顶部工具按钮 */
    QPushButton* mBtnTool;
};

#endif // CHATVIEWUI_H
