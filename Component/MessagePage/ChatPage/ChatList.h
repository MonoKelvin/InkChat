#ifndef CHATLIST_H
#define CHATLIST_H

#include <AbstractChatListItem.h>

#include <QAbstractListModel>
#include <QPointer>

class ChatItem;
class IChatObject;

class ChatList : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ChatList)

    friend class MessageDatabase;
    friend class MessageManager;
public:
    explicit ChatList(QObject* parent = nullptr);
    ~ChatList() override;

    /**
     * @brief 在指定位置插入一条聊天项
     * @param row 行号
     * @param chat 聊天项
     * @return bool 插入成功返回true，否则返回false
     * @warning 实际情况中尽量不要使用该函数，因为消息的增加几乎发生在从聊天视图的底部推送。
     * @see sendChat、appendChat
     */
    void insertChat(int row, AbstractChatListItem* item);

    /**
     * @brief 给定行号返回聊天项
     * @param row 聊天项所在行数
     * @param onlyChat 是否只计算聊天消息的行数
     * @return 返回聊天项
     */
    AbstractChatListItem* getChatItem(int row, bool onlyChat = false) const;

    /**
     * @brief 给定聊天项获得其所在行数
     * @param chat 给定的聊天项
     * @return 聊天项所在行数，若不存在则返回-1
     */
    inline int getRow(AbstractChatListItem* message) const
    {
        return mChats.indexOf(message);
    }

    /**
     * @brief 移除给定索引的聊天项
     * @param row 行索引，即聊天项所在行数
     * @param cascade 级联选项，如果为true则同步删除本地数据库中对应的聊天记录，否则仅仅
     * 删除聊天视图中的记录。
     */
    void removeChatItem(int row, bool cascade = false);

    /**
     * @brief 给定父元素下的行数量，即聊天总数
     * @param parent 指定的父元素
     * @return 行数
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @brief 获取当前聊天视图的聊天对象
     * @return IChatObject* 返回聊天对象的指针
     */
    IChatObject* getChatObject() const noexcept;

    /**
     * @brief 直接在尾部追加一条项目，不会改变数据库的内容
     * @param item 要追加的项目
     * @see sendChat
     */
    inline void appendItem(AbstractChatListItem* item)
    {
        insertChat(mChats.size(), item);
    }

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    //inline QHash<int, QByteArray> roleNames() const override
    //{
    //}

Q_SIGNALS:
    void failed(const QString& msg);

    /**
     * @brief 信号：用户没有找到，即不是我的好友发送的消息
     * @param item 聊天信息
     */
    //void userMissed(IChatItem* item);

    /** 
     * @brief 信号：当聊天消息增加时
     * @param row 增加时的行数
     */
    void chatAdded(int row);

public Q_SLOTS:
    /**
     * @brief 加载更多
     * @param parent 可以忽视该参数
     */
    void fetchMore(const QModelIndex& parent = QModelIndex()) override;

    /**
     * @brief 清空所有的聊天记录，不会清空本地聊天数据库中的对应记录。
     */
    void clear();

    /**
     * @brief 是否能够发送该消息到视图中
     * @param item 准备发送的消息项。内部实现方法的不同可能会将该item抛弃
     * @todo TODO: next version
     */
    //virtual bool canSend(ChatItem* item);

    /**
     * @brief 初始化加载与给定用户的聊天消息
     * @param chatObj 聊天对象
     * @note 一般来说只用一次
     */
    void initLoad(IChatObject* chatObj);

private:
    /** 所有的聊天消息容器 */
    QList<QPointer<AbstractChatListItem>> mChats;

    /** 指向当前聊天对象的指针 */
    QPointer<IChatObject> mChatObject;
};

#endif // CHATLIST_H
