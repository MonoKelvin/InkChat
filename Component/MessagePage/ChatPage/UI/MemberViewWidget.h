#ifndef MEMBERVIEWWIDGET_H
#define MEMBERVIEWWIDGET_H

#include <QWidget>

class QListView;
class QPushButton;
class MemberList;
class LanObject;

class MemberViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit MemberViewWidget(LanObject* lan, QWidget* parent = nullptr);

    inline MemberList* getMemberListModel(void) const noexcept
    {
        return mMemberListModel;
    }

private Q_SLOTS:
    void updateChatEnabled(const QModelIndex& index);
    void requestChatPage();
    void refresh();

private:
    void resizeEvent(QResizeEvent*) override;

private:
    /** 成员列表 */
    QListView* mMemberList;

    /** 阴影背景 */
    QWidget* mShadowWidget;

    /** 成员列表视图 */
    MemberList* mMemberListModel;

    /** 进行聊天的按钮 */
    QPushButton* mBtnChat;
};

#endif // MEMBERVIEWWIDGET_H
