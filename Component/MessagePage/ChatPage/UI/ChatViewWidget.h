#ifndef CHATVIEWWIDGET_H
#define CHATVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class ChatViewWidget;
}

class ChatList;
class IChatItem;
class IChatObject;

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
