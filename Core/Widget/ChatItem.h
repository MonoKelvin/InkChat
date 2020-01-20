#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;
class Avatar;
class User;

class ChatItem : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItem(QWidget *parent = nullptr);
    explicit ChatItem(QSharedPointer<User> user, const QString &msg, const QString &time, QWidget *parent = nullptr);

    enum class EUserType
    {
        System, // 系统
        Me, // 自己
        Other, // 其他人
        Time, // 时间
    };

    enum class EChatSendState
    {
        Sending, // 正在发送
        Succeed, // 发送成功
        Failed, // 发送失败
        Withdraw, // 撤回
    };

    inline const QString getMessage(void) const;

    inline const QString getTime(void) const;

    inline EUserType getUserType() { return mUserType; }

private:
    void init();

private:
    /** 消息发送者的头像 */
    Avatar *mAvatar;

    /** 消息发送者的名称 */
    QString mName;

    /** 发送的消息控件 */
    QLabel *mMessage;

    /** 消息发送并接收到的时间控件 */
    QLabel *mTime;

    /** 发送消息的对象 */
    EUserType mUserType;

    /** 正在加载控件 */
    QLabel *mLoading;

    /** 消息发送状态 */
    EChatSendState mSendState;
};

#endif // CHATITEM_H

