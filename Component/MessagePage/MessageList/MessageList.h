#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QAbstractListModel>
#include <QPointer>

class QMutex;
class IChatObject;
class MessageItem;

class MessageList : public QAbstractListModel {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(MessageList)

    friend class MessageDatabase;
    friend class MessageManager;

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

    /**
     * @brief 消息排序类型
     * @todo Next Version
     */
    enum EMessageSortType {
        SortByTopOrder = 0x0001,
        SortByName = 0x0002,
        SortByTime = 0x0004,
        SortByMessageCount = 0x0008,
    };

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
    inline int getRow(MessageItem* message) const
    {
        return mMessages.indexOf(message);
    }

    /**
     * @brief 移除给定索引的消息项
     * @param index 行索引，即消息所在行数
     */
    void removeMessage(int index);

    /**
     * @brief 在指定位置插入一条消息
     * @param index 行号
     * @param message 消息项
     */
    void insertMessage(int index, MessageItem* message);

    /**
     * @brief 移动消息位置
     * @param from 起始行索引
     * @param to 目的行索引
     */
    void moveMessage(int from, int to);

    /**
     * @brief 在最后添加一条消息
     * @param message 消息项
     */
    inline void appendMessage(MessageItem* message)
    {
        return insertMessage(mMessages.size(), message);
    }

    /**
     * @brief 是否存在与指定聊天对象的消息项
     * @param chatObj 聊天对象实例
     * @return 如果存在与指定聊天对象的消息项则返回true，否则返回false
     */
    bool isChatObjectExists(IChatObject* chatObj);

    /**
     * @brief 设置消息项的是否置顶
     * @param message 消息项
     * @param isTop 是否置顶，true时置顶，否则为取消置顶
     * @param moveToBottom 是否移动到最底部
     * @note isTop 会受 moveToBottom 影响。
     * 如果moveToBottom为true：
     *  isTop = true: item将移动到所有置顶项的最下方
     *  isTop = false: 移动到所有项的最下方
     * 如果moveToBottom为false：
     *  isTop = true: item将移动到所有置顶项的最上方
     *  isTop = false: 移动到所有项的最上方
     * 
     * TODO：目前不支持moveToBottom参数的作用
     */
    void setMessageTop(MessageItem* message, bool isTop = true, bool moveToBottom = false);

    /**
     * @brief 给定父元素下的行数量，即消息总数
     * @param parent 指定的父元素，如果父元素为空，即直接传入QModelIndex()，
     * 得到的是所有元素的数量
     * @return 行数
     */
    inline int rowCount(const QModelIndex& parent) const override
    {
        Q_UNUSED(parent)
        return mMessages.size();
    }

    //MessageItem* getCurrentSelectedItem(void) const;

    //static QSharedPointer<IChatObject> GetCurrentChatObject(void);

Q_SIGNALS:
    void saved();
    void loaded();
    void failed(const QString&);

public Q_SLOTS:
    void fetchMore(const QModelIndex& index = QModelIndex()) override;

    /**
     * @brief 加载缓存的聊天数据
     * @note 该槽函数内没有设置新的线程，所以尽量使用信号槽机制来完成异步加载。
     */
    void load(void /*, EMessageFilter filter = NoFilter*/);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QHash<int, QByteArray> roleNames(void) const override;

    /**
     * @brief 设置当前选中item的索引
     * @param index 选中item的索引
     * @note 设置后并不会直观的改变列表界面中任何显示的内容
     */
    //void setCurrentSelectedIndex(int index);

    /**
     * @brief 获得当前选中的item的索引
     * @return int 选中item的索引
     */
    //int getCurrentSelectedIndex(void);

    /**
     * @brief 调整给定的消息
     * @note 该message必须存在于列表中
     * @return 调整完后返回调整后的行索引，调整失败返回-1
     * TODO: 目前只支持按置顶顺序调整
     */
    //int adjust(MessageItem* message /* EMessageSortType type = SortByTopOrder */);

    /**
     * @brief 提升未置顶消息到未置顶消息的最上方。如果已经是置顶消息，则提升到所有消息的最上方。
     * @param message 要提升的消息项
     * @warning 使用该方法时必须保证所有置顶消息在最上方
     */
    void ariseMessage(MessageItem* message);

private:
    /** 消息记录文件夹 */
    QString mCrFolder;

    /** 消息 */
    QList<MessageItem*> mMessages;

    /** 
     * @brief 当前被选中的item
     * @note 方便更新列表时也更新item索引，同时判断当前打开的聊天视图，在收发数据时数据有用
     */
    //static QPointer<MessageItem> mCurrentSelectedItem;
};

#endif // MESSAGELIST_H
