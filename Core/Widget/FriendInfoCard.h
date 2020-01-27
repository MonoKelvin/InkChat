#ifndef FRIENDINFOCARD_H
#define FRIENDINFOCARD_H

#include <QWidget>

class User;
class QLabel;
class QPushButton;

class FriendInfoCard : public QWidget
{
    Q_OBJECT
public:
    explicit FriendInfoCard(QSharedPointer<User> user, QWidget *parent = nullptr);

signals:

private:

    /** 头像 */
    QLabel *mAvatar;

    /** 编辑备注按钮 */
    QPushButton *mEditRemark;

    /** 设置分组按钮 */
    QPushButton *mSetGroup;

    /** 删除好友按钮 */
    QPushButton *mDeleteFriend;
};

#endif // FRIENDINFOCARD_H
