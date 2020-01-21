#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>

class QLabel;
class Avatar;
class User;

class ChatItem : public QWidget
{
    Q_OBJECT

    friend class ChatListView;
public:

    /**
     * @brief 发送聊天消息的发送方枚举
     */
    enum class ESender
    {
        System, // 系统
        Me, // 自己
        Ta, // ta（他或她，一个人）
        Other, // 其他人（针对群聊天）
    };

    /**
     * @brief 聊天消息发送状态枚举
     */
    enum class ESendState
    {
        Sending, // 正在发送
        Succeed, // 发送成功
        Failed, // 发送失败
        Withdraw, // 撤回
    };

public:
    explicit ChatItem(QSharedPointer<User> user, const QString &msg, ESender sender = ESender::Ta, QWidget *parent = nullptr);

    /**
     * @brief 设置聊天内容
     * @param text 聊天消息
     */
    void setMessage(const QString &text);

    /**
     * @brief 设置发送时间，这个时间是接收方收到消息的时间，如果发送失败，就显示发送时间
     * @param time
     */
    void setTime(const QString &time);

    inline const QString getMessage(void) const;

    inline const QString getTime(void) const;

    inline ESender getUserType() { return mSender; }

private:
    void init();

    void updateHeight();

private:
    /** 消息发送者的头像 */
    Avatar *mAvatar;

    /** 消息发送者的名称 */
    QString mName;

    /** 发送的消息控件 */
    QLabel *mMessage;

    /** 消息发送并接收到的时间控件 */
    QLabel *mTime;

    /** 发送消息的对象类型 */
    ESender mSender;

    /** 发送状态控件 */
    QLabel *mSendStateLabel;

    /** 消息发送状态 */
    ESendState mSendState;
};

#endif // CHATITEM_H

