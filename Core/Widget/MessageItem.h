#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QWidget>

class QLabel;
class Avatar;
class Badge;

class MessageItem : public QWidget
{
    Q_OBJECT

    friend class MessageList;
public:
    explicit MessageItem(unsigned int id, QWidget *parent = nullptr);

    void setName(const QString &name);
    void setMessage(const QString &message);

    void setUnreadMessageNumber(unsigned int number);
    unsigned int getUnreadMessageNumber(void) const { return mMessageNumber; }

signals:
    void onRead(unsigned int id);

protected slots:
    void readMessage();
    void unreadMessage();

protected:
    virtual void updateContents();

    /**
     * @brief 设置badge的内容
     * @param text 要设置的内容
     * @note 该方法首先会判断text是否为数字x
     * 1.如果0<x<100，则badge显示x
     * 2.如果x>99，则badge显示99+
     * 3.如果x=0，则badge会被删除
     * 如果text不是数字，则badge被设置成text的值
     * @note 可以重写该方法实现自己的算法
     */
    virtual void setBadge(const QString& text);

private:
    /** 头像控件 */
    Avatar *mAvatar;

    /** 显示的名称，如果是用户则为昵称或备注名，如果是群则为群名 */
    QLabel *mName;

    /** 显示的消息概要，超过item长度以...的形式结尾 */
    QLabel *mMessage;

    /** 显示最近发送消息的时间或消息条数 */
    Badge *mBadge;

    /** 未读消息数量 */
    unsigned int mMessageNumber;

    /** 这条消息在数据库中的ID，一般是好友、群ID */
    unsigned int mID;

    /** 是否已读 */
    bool mIsRead;
};

#endif // MESSAGEITEM_H
