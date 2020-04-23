#ifndef CHATVIEWWIDGET_H
#define CHATVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class ChatViewWidget;
}

class ChatList;
class QListView;

class ChatViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatViewWidget(QWidget* parent = nullptr);
    ~ChatViewWidget() override;

    ChatList* getChatListModel(void) const
    {
        return mChatListModel;
    }

    QListView* getChatView() const;

private:
    void resizeEvent(QResizeEvent*) override;

public Q_SLOTS:
    /**
     * @brief 发送普通的文本消息
     * @param msg 文本消息
     */
    void sendMessage(const QString& msg);

private:
    Ui::ChatViewWidget* ui;

    ChatList* mChatListModel;
};

#endif // CHATVIEWWIDGET_H
