#ifndef MEMBERLISTVIEWWIDGET_H
#define MEMBERLISTVIEWWIDGET_H

#include <QWidget>

class QListView;
class QPushButton;
class MemberList;

class MemberListViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemberListViewWidget(QWidget* parent = nullptr);

    void popup(const QPoint& pos);

    inline MemberList* getMemberListModel(void) const noexcept
    {
        return mMemberListModel;
    }

private Q_SLOTS:
    void updateChatEnabled(const QModelIndex& index);
    void requestChatPage(void);

private:
    void showEvent(QShowEvent*) override;

private:
    /** 成员列表 */
    QListView* mMemberList;

    /** 成员列表视图 */
    MemberList* mMemberListModel;

    /** 关闭面板的按钮 */
    QPushButton* mBtnClose;

    /** 进行聊天的按钮 */
    QPushButton* mBtnChat;
};

#endif // MEMBERLISTVIEWWIDGET_H
