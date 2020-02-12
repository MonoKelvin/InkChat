#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <IChatObject.h>

#define MESSAGEITEM_INITIALIZA                                            \
    {                                                                     \
        qmlRegisterType<MessageItem>("MessageItem", 1, 0, "MessageItem"); \
    }

class MessageItem : public QObject {
    Q_OBJECT
    friend class MessageList;

    Q_PROPERTY(bool dirty READ getDirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(bool readFlag READ getReadFlag WRITE setReadFlag NOTIFY readFlagChanged)
    Q_PROPERTY(IChatObject* chatObject READ getChatObject CONSTANT)
    Q_PROPERTY(QString time READ getTime WRITE setTime)
    Q_PROPERTY(QString message READ getMessage WRITE setMessage)
    Q_PROPERTY(int unreadMsgCount READ getUnreadMsgCount WRITE setUnreadMsgCount NOTIFY unreadMsgCountChanged)

public:
    explicit MessageItem(QObject* parent = nullptr);
    explicit MessageItem(IChatObject* chatObject, QObject* parent = nullptr);

    inline bool getDirty(void) const { return mDirty; }
    inline void setDirty(bool dirty)
    {
        mDirty = dirty;
        emit dirtyChanged();
    }

    IChatObject* getChatObject() const { return mChatObject; }

    inline QString getTime(void) const { return mTime; }
    inline void setTime(const QString& time) { mTime = time; }

    inline QString getMessage(void) const { return mMessage; }
    inline void setMessage(const QString& message) { mMessage = message; }

    inline bool getReadFlag(void) const { return mReadFlag; }
    inline void setReadFlag(bool readFlag) { mReadFlag = readFlag; }

    inline int getUnreadMsgCount(void) const { return mUnreadMsgCount; }
    inline void setUnreadMsgCount(int count) { mUnreadMsgCount = count; }

Q_SIGNALS:
    void dirtyChanged();
    void readFlagChanged();
    void unreadMsgCountChanged();

private:
    /** 脏位。只有内部数据修改后该值位true，否则位false，方便保存减少数据的保存 */
    bool mDirty;

    /** 是否已经读过消息了 */
    bool mReadFlag;

    /** 消息简要 */
    QString mMessage;

    /** 最近消息的时间 */
    QString mTime;

    /** 未读消息数量 */
    int mUnreadMsgCount;

    /** 聊天对象，可以是单用户，也可以是局域网 */
    IChatObject* mChatObject;
};

#endif // MESSAGEITEM_H
