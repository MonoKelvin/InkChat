#ifndef CHATVIEWWIDGET_H
#define CHATVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class ChatViewWidget;
}

class ChatList;
class IChatItem;

class ChatViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatViewWidget(QWidget *parent = nullptr);
    ~ChatViewWidget() override;

public Q_SLOTS:
    /**
     * @brief 当发送（普通文本）消息时
     * @param msg 要发送的文本消息
     */
    void onSend(const QString& msg);

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    /**
     * @brief 更新视图
     */
    void updateViewport(void);

private:
    Ui::ChatViewWidget* ui;

    ChatList* mChatListModel;
};

#endif // CHATVIEWWIDGET_H
