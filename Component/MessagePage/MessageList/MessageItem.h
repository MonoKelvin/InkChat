﻿#ifndef MESSAGEITEM_H
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

    inline IChatObject* getChatObject() const { return mChatObject; }

    /**
     * @brief 设置聊天对象
     * @param chatObject 聊天对象类，内容不可为空
     * @see void setChatObject(IChatObject* chatObject)
     */
    void setChatObject(IChatObject* chatObject);

    inline const QString getTime(void) const { return mTime; }
    void setTime(const QDateTime& time) noexcept;

    inline const QString getMessage(void) const { return mMessage; }
    inline void setMessage(const QString& message) { mMessage = message; }

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
    void readFlagChanged();
    void unreadMsgCountChanged();

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
    QPointer<IChatObject> mChatObject;
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
