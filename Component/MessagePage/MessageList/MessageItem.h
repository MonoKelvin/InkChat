#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <IChatObject.h>
#include <QJsonObject>

#define MESSAGEITEM_INITIALIZA                                            \
    {                                                                     \
        qmlRegisterType<MessageItem>("MessageItem", 1, 0, "MessageItem"); \
    }

class QFileInfo;

class MessageItem : public QObject {
    Q_OBJECT
    friend class MessageList;

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

    IChatObject* getChatObject() const { return mChatObject; }

    inline QString getTime(void) const { return mTime; }

    inline QString getMessage(void) const { return mMessage; }

    inline bool getReadFlag(void) const { return mReadFlag; }
    inline void setReadFlag(bool readFlag) { mReadFlag = readFlag; }

    inline int getUnreadMsgCount(void) const { return mUnreadMsgCount; }
    void setUnreadMsgCount(int count);

Q_SIGNALS:
    void dirtyChanged();
    void readFlagChanged();
    void unreadMsgCountChanged();

public slots:
    void update();

private:
    /**
     * @brief 加载缓存消息
     * @param fileInfo 文件信息
     * @return bool 当不需要加载，即虽然内部有数据，但无需在界面中显示时返回false，否则返回true。
     * @note 该函数内部解析出错将会抛出异常，所以外部应设置try-catch块
     * @warning 另外，该函数只应该被调用一次用以初始化内部数据，当任何数据被修改需要更新到文件时
     * 请使用 @see update 函数。
     */
    bool load(const QFileInfo& fileInfo);

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

    /** 保存一个JsonObject对象，方便写入数据保存到文件 */
    QJsonObject mJsonObject;
};

#endif // MESSAGEITEM_H
