#ifndef CHATITEM_H
#define CHATITEM_H

#include "IChatWidget.h"

class QLabel;
class Avatar;
class User;

class ChatItem : public IChatWidget
{
    Q_OBJECT

    friend class ChatListView;
public:
    explicit ChatItem(QSharedPointer<User> user, const QString &msg, ESender sender = ESender::Me, QWidget *parent = nullptr);

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

    /**
     * @brief 设置发送者类型
     * @param sender 发送者枚举类型
     * @note 只有设置为ESender::Me，气泡在聊天视图中才显示在右边，其他的都是在左边
     */
    void setSender(ESender sender) override;

    inline const QString getMessage(void) const;

    inline const QString getTime(void) const;

protected:
    void updateContents() override;

private:
    /** 消息发送者的头像 */
    Avatar *mAvatar;

    /** 消息发送者的名称控件，只有群聊才有内容，否则只是空的占位 */
    QLabel *mName;

    /** 发送的消息控件 */
    QLabel *mMessage;

    /** 消息发送并接收到的时间控件 */
    QLabel *mTime;

    /** 发送状态控件 */
    QLabel *mSendStateLabel;

};

#endif // CHATITEM_H

