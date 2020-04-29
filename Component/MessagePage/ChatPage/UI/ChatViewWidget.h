#ifndef CHATVIEWWIDGET_H
#define CHATVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class ChatViewWidget;
}

class ChatList;
class QListView;
class LoadingIndicator;

class ChatViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatViewWidget(QWidget* parent = nullptr);
    ~ChatViewWidget() override;

    void setTitle(const QString& name) noexcept;

    ChatList* getChatListModel(void) const noexcept
    {
        return mChatListModel;
    }

    QListView* getChatView(void) const;

public Q_SLOTS:
    /**
     * @brief 发送普通的文本消息
     * @param int 类型
     * @param msg 消息
     */
    void sendChat(int type, const QVariant& msg) noexcept;

    /**
     * @brief 由内部算法自动决定是否应该滚动到底部
     */
    void autoDetermineScrollToBottom();

private Q_SLOTS:
    void builChatViewWidgetMenu();
    void clearRecord();
    void openChatObjectInfo();

private:
    void resizeEvent(QResizeEvent*) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::ChatViewWidget* ui;

    /** 聊天列表视图模型 */
    ChatList* mChatListModel;

    /** 加载指示器 */
    LoadingIndicator* mLoader;

    /** 鼠标点击时的点 */
    QPoint mPressedPoint;
};

#endif // CHATVIEWWIDGET_H
