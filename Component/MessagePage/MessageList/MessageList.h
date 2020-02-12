#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QAbstractListModel>

#define MESSAGELIST_INITIALIZA                                                      \
    {                                                                               \
        qmlRegisterType<MessageList>("MessageListModel", 1, 0, "MessageListModel"); \
    }

class QMutex;
class IChatObject;
class MessageItem;

class MessageList : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MessageList(QObject* parent = nullptr);
    ~MessageList() override;

    /**
     * @brief 消息保存或加载时的过滤器
     * @todo Next Version
     */
    enum EMessageFilter {
        FriendMessages = 0x0001,
        LANMessage = 0x0002,
        GroupMessages = 0x0004,
        NoFilter = FriendMessages | LANMessage | GroupMessages,
    };
    Q_ENUM(EMessageFilter)

    //    QHash<int, QByteArray> roleNames() const override;
    //    bool removeRows(int row, int count, const QModelIndex& parent) override;
    //    void fetchMore(const QModelIndex& parent) override;
    //    bool canFetchMore(const QModelIndex& parent) const override;
    //    void sort(int column, Qt::SortOrder order) override;
    //    QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const override;

    /**
     * @brief 清空所有的消息
     */
    void clearMessage();

    /**
     * @brief 给定索引返回消息
     * @param index 行索引，即消息所在行数
     * @return 返回消息项
     */
    MessageItem* getMessage(int index) const;

    /**
     * @brief 给定消息获得其所在行数
     * @param message 给定的消息
     * @return 消息所在行数，消息不存在则返回-1
     */
    int getRow(MessageItem* message) const;

    /**
     * @brief 移除给定索引的消息项
     * @param index 行索引，即消息所在行数
     */
    void removeMessage(int index);

    /**
     * @brief 在指定位置插入一条消息
     * @param index 行号
     * @param message 消息项
     * @return bool 插入成功返回true，否则返回false
     */
    bool insertMessage(int index, MessageItem* message);

    inline bool appendMessage(MessageItem* message)
    {
        return insertMessage(mMessages.size(), message);
    }

    /**
     * @brief 给定父元素下的行数量，即消息总数
     * @param parent 指定的父元素，如果父元素为空，即直接传入QModelIndex()，
     * 得到的是所有元素的数量
     * @return 行数
     */
    int rowCount(const QModelIndex& parent) const override;

Q_SIGNALS:
    void saved();
    void loaded();
    void failed(const QString&);

public Q_SLOTS:
    /**
     * @brief 接收来自指定聊天对象的消息
     * @param chatObj 消息来源的聊天对象
     * @param message 接收的消息
     * @param time 接收的时间
     * @note 该槽函数内没有设置新的线程，所以尽量使用信号槽机制来完成异步接收。
     */
    void receiveMessage(IChatObject* chatObj, const QString& message, const QString& time);

    /**
     * @brief 提升消息到顶部
     * @param message 要提升的消息项
     * @param ignoreTop 是否忽略置顶的消息，如果忽略则提升为最上，否则提升到未置顶消息的最上方。
     * @return 提升成功返回true，否则返回false
     */
    bool ariseMessage(MessageItem* message, bool ignoreTop = false);

    bool refresh();

    /**
     * @brief 加载缓存的聊天数据
     * @param crFolder 缓存文件所在的文件夹，只能识别 *.cr 文件
     * @note 该槽函数内没有设置新的线程，所以尽量使用信号槽机制来完成异步加载。
     */
    void load(const QString& crFolder /*, EMessageFilter filter = NoFilter*/);
    void save(const QString& crFolder, bool onlyDirty = true /*, EMessageFilter filter = NoFilter*/);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

private:
    /** 消息记录文件夹 */
    QString mCrFolder;

    /** 消息 */
    QList<MessageItem*> mMessages;

    QMutex* mMutex;
};

#endif // MESSAGELIST_H
