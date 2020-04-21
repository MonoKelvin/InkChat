#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <IChatObject.h>

#include <QAbstractItemDelegate>
#include <QPointer>

class QFileInfo;

class MessageItem : public QObject {
    Q_OBJECT

    friend class MessageList;
    friend class MessageDatabase;
    friend class MessageItemDelegate;

public:
    explicit MessageItem(QObject* parent = nullptr);
    ~MessageItem() override;

    IChatObject* getChatObject() const { return mChatObject.get(); }

    /**
     * @brief 设置聊天对象
     * @param chatObject 聊天对象类，内容不可为空
     * @see void setChatObject(IChatObject* chatObject)
     */
    void setChatObject(QSharedPointer<IChatObject> chatObject);

    /**
     * @brief 设置聊天对象
     * @param chatObject 聊天对象类，内容不可为空
     * @see void setChatObject(QSharedPointer<IChatObject> chatObject)
     */
    inline void setChatObject(IChatObject* chatObject)
    {
        setChatObject(QSharedPointer<IChatObject>(chatObject));
    }

    inline const QString getTime(void) const { return mTime; }

    inline const QString getMessage(void) const { return mMessage; }
    // void setMessage(const QString& message);

    inline bool isTop(void) const { return mIsTop; }
    inline void setIsTop(bool top)
    {
        if (top != mIsTop) {
            mIsTop = top;
            emit topChanged();
        }
    }

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
    void topChanged();
    void dirtyChanged();
    void readFlagChanged();
    void unreadMsgCountChanged();

private Q_SLOTS:
    void onTopChanged();

private:
    /** 是否置顶 */
    bool mIsTop;

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

class MessageItemDelegate : public QAbstractItemDelegate {
public:
    explicit MessageItemDelegate(QObject* parent = nullptr)
        : QAbstractItemDelegate(parent)
    {
    }

    enum { MessageItemRole = Qt::UserRole + 100 };

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // MESSAGEITEM_H
