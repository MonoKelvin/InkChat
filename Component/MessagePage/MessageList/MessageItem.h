#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <IChatObject.h>
#include <QJsonObject>
#include <QPointer>

class QFileInfo;

class MessageItem : public QObject {
    Q_OBJECT

    friend class MessageList;
    friend class MessageDatabase;

    Q_PROPERTY(bool dirty READ getDirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(bool readFlag READ getReadFlag WRITE setReadFlag NOTIFY readFlagChanged)
    Q_PROPERTY(QString time READ getTime CONSTANT)
    Q_PROPERTY(QString message READ getMessage CONSTANT)
    Q_PROPERTY(int unreadMsgCount READ getUnreadMsgCount WRITE setUnreadMsgCount NOTIFY unreadMsgCountChanged)
    Q_PROPERTY(IChatObject* chatObject READ getChatObject CONSTANT)

public:
    explicit MessageItem(QObject* parent = nullptr);
    ~MessageItem();

    inline bool getDirty(void) const { return mDirty; }
    inline void setDirty(bool dirty)
    {
        mDirty = dirty;
        emit dirtyChanged();
    }

    IChatObject* getChatObject() const { return mChatObject.get(); }

    inline const QString getTime(void) const { return mTime; }

    inline const QString getMessage(void) const { return mMessage; }
    // void setMessage(const QString& message);

    inline bool getReadFlag(void) const { return mReadFlag; }
    inline void setReadFlag(bool readFlag)
    {
        if (readFlag != mReadFlag) {
            setDirty(true);
            mReadFlag = readFlag;
            emit readFlagChanged();
        }
    }

    inline int getUnreadMsgCount(void) const { return mUnreadMsgCount; }
    void setUnreadMsgCount(int count);

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
    QSharedPointer<IChatObject> mChatObject;
};

#endif // MESSAGEITEM_H
