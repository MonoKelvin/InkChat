#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <IChatItem.h>

#include <QAbstractListModel>

class ChatView : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ChatView)

    friend class MessageDatabase;

public:
    explicit ChatView(QObject* parent = nullptr);
    ~ChatView() override;

    /**
     * @brief 注册一个聊天类。这样在就可使用自定义的聊天类发送或接收消息
     */
    template <typename T>
    static inline void RegisterChatItemClass()
    {
        const auto metaClass = T::staticMetaObject;

        Q_ASSERT_X(metaClass.inherits(&IChatItem::staticMetaObject), "REGISTER_CLASS_FAILED", "非继承自IChatItem的类无法注册");

        if (mRegistryChatClasses.contains(T::ChatType)) {
            qWarning("聊天类型已存在，原值将会被覆盖");
        }

        qRegisterMetaType<T>(metaClass.className());
        mRegistryChatClasses.insert(T::ChatType, metaClass.className());
    }

    /**
     * @brief 清空所有的聊天记录，同时也会清空本地聊天数据库中的对应记录。谨慎使用
     * @note 如果只清空视图界面的聊天元素，不关联数据库，使用 @see clear
     */
    void clearChatRecord();

    /**
     * @brief 清空所有的聊天记录，不会清空本地聊天数据库中的对应记录。
     * @note 如果还要清空本地聊天数据库中的对应记录，使用 @see clearChatRecord
     */
    void clear();

    /**
     * @brief 给定行号返回聊天项
     * @param row 聊天项所在行数
     * @return 返回聊天项
     */
    IChatItem* getChatItem(int row) const;

    /**
     * @brief 给定聊天项获得其所在行数
     * @param chat 给定的聊天项
     * @return 聊天项所在行数，若不存在则返回-1
     */
    inline int getRow(IChatItem* message) const
    {
        return mChats.indexOf(message);
    }

    /**
     * @brief 移除给定索引的聊天项
     * @param row 行索引，即聊天项所在行数
     * @param cascade 级联选项，如果为true则同步删除本地数据库中对应的聊天记录，否则仅仅
     * 删除聊天视图中的记录。
     */
    void removeChatItem(int row, bool cascade = true);

    /**
     * @brief 给定父元素下的行数量，即聊天总数
     * @param parent 指定的父元素
     * @return 行数
     */
    int rowCount(const QModelIndex& parent) const override;

    /**
     * @brief 直接在尾部追加一条消息，不会改变数据库的内容
     * @param chat 要追加的聊天消息
     */
    inline void appendChat(IChatItem* chat)
    {
        insertChat(mChats.size(), chat);
    }

public Q_SLOTS:
    /**
     * @brief 发送一条聊天消息，并会改变数据库的内容
     * @param message 消息项
     * @return 发送成功并更新数据库成功返回true，否则返回false
     */
    bool sendChat(IChatItem* chat);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    inline QHash<int, QByteArray> roleNames() const override
    {
        mRegistryChatClasses.insert(0, QByteArrayLiteral("chatItem"));
        return mRegistryChatClasses;
    }

    /**
     * @brief 在指定位置插入一条聊天项
     * @param row 行号
     * @param chat 聊天项
     * @return bool 插入成功返回true，否则返回false
     * @warning 实际情况中尽量不要使用该函数，因为消息的增加几乎发生在从聊天视图的底部推送。
     * @see sendChat
     */
    bool insertChat(int row, IChatItem* chat);

    /**
     * @brief 通过给定聊天类类型构建一个聊天控件
     * @param chatType 聊天类型 @see IChatItem::ChatType
     * @return 返回创建好的聊天类，如果创建失败则返回nullptr
     * @note 该方法构建的聊天消息不会推送到聊天视图中
     */
    IChatItem* buildChatItem(int chatType);

public Q_SLOTS:
    /**
     * @brief 加载与给定用户id的聊天消息
     * @param id 与“我”聊天的用户id
     */
    void load(unsigned int id);

Q_SIGNALS:
    /** 信号：任何失败消息产生时将会发送的信号 */
    void failed(const QString& msg);

private:
    /** 所有的聊天消息容器 */
    QList<IChatItem*> mChats;

    /**
     * @brief 注册的聊天类容器
     * @type int 表示类的 ChatType
     * @type QByteArray 表示类的名称，即在qml中可以访问的类名
     */
    static QHash<int, QByteArray> mRegistryChatClasses;
};

#endif // CHATVIEW_H
