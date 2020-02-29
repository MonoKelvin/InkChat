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

    Q_PROPERTY(bool readFlag READ getReadFlag WRITE setReadFlag NOTIFY readFlagChanged)
    Q_PROPERTY(QString time READ getTime CONSTANT)
    Q_PROPERTY(QString message READ getMessage CONSTANT)
    Q_PROPERTY(int unreadMsgCount READ getUnreadMsgCount WRITE setUnreadMsgCount NOTIFY unreadMsgCountChanged)
    Q_PROPERTY(IChatObject* chatObject READ getChatObject CONSTANT)

public:
    explicit MessageItem(QObject* parent = nullptr);
    ~MessageItem();

    IChatObject* getChatObject() const { return mChatObject.get(); }

    /**
     * @brief 设置聊天对象
     * @param chatObject 聊天对象类，内容不可为空
     * @see void setChatObject(IChatObject* chatObject)
     * @warning 该方法只可使用一次。即之后不能够再设置聊天对象类
     */
    void setChatObject(QSharedPointer<IChatObject> chatObject);

    /**
     * @brief 设置聊天对象
     * @param chatObject 聊天对象类，内容不可为空
     * @see void setChatObject(QSharedPointer<IChatObject> chatObject)
     * @warning 该方法只可使用一次。即之后不能够再设置聊天对象类
     */
    inline void setChatObject(IChatObject* chatObject)
    {
        setChatObject(QSharedPointer<IChatObject>(chatObject));
    }

    inline const QString getTime(void) const { return mTime; }

    inline const QString getMessage(void) const { return mMessage; }
    // void setMessage(const QString& message);

    inline bool getReadFlag(void) const { return mReadFlag; }
    inline void setReadFlag(bool readFlag)
    {
        if (readFlag != mReadFlag) {
            mReadFlag = readFlag;
            emit readFlagChanged();
        }
    }

    inline int getUnreadMsgCount(void) const { return mUnreadMsgCount; }
    inline void setUnreadMsgCount(int count)
    {
        if (count != mUnreadMsgCount) {
            mUnreadMsgCount = count;
            emit unreadMsgCountChanged();
        }
    }

Q_SIGNALS:
    void dirtyChanged();
    void readFlagChanged();
    void unreadMsgCountChanged();

private Q_SLOTS:
    void onTopChanged();

private:
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
