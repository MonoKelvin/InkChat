#ifndef ICHATWIDGET_H
#define ICHATWIDGET_H

#include <QWidget>

class IChatWidget : public QWidget
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
    Q_ENUM(ESender)

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
    Q_ENUM(ESendState)

public:
    explicit IChatWidget(ESender sender = ESender::Me, QWidget *parent = nullptr);

    inline ESender getSender() const { return mSender; }

    virtual void setSender(ESender) {}

//Q_SIGNALS:
//    void onSentSuccessful();

protected:
    inline void resizeEvent(QResizeEvent *) override
    {
        mIsUpdatedContents = false;
    }

    /**
     * @brief 更新内容方法
     */
    virtual void updateContents(void) = 0;

//protected slots:
//    virtual void sendStateChanged(ESendState state);

protected:
    /** 发送消息的对象类型 */
    ESender mSender;

    /** 消息发送状态 */
    ESendState mSendState;

    /** 是否更新过内容区域了，用于减少更新次数，提高效率 */
    bool mIsUpdatedContents;

};

#endif // ICHATWIDGET_H
