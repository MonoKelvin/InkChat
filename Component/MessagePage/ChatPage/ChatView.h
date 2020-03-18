#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <IChatItem.h>

#include <QAbstractListModel>

class ChatView : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ChatView)

    friend class MessageDatabase;
    friend class MessageManager;

    Q_PROPERTY(IChatObject* chatObject READ getChatObject CONSTANT)

public:
    explicit ChatView(QObject* parent = nullptr);
    ~ChatView() override;

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
     * @brief 通过给定聊天类类型构建一个空的聊天控件
     * @param chatType 聊天类型 @see IChatItem::ChatType
     * @param isMe 是否是我发送的消息
     * @param uid 用户id
     * @return 返回创建好的聊天类，如果创建失败则返回nullptr
     * @note 该方法构建的聊天消息不会推送到聊天视图中
     */
    static IChatItem* BuildChatItem(int chatType, bool isMe, unsigned int uid);

    /**
     * @brief 构建一个“我”发送的聊天控件
     * @param chatType 聊天类型 @see IChatItem::ChatType
     * @param time 发送时间
     * @param data 发送的数据
     * @return 返回创建好的聊天类，如果创建失败则返回nullptr
     * @note 注意，构建好的聊天数据不会保存到数据库
     */
    static IChatItem* BuildChatItem(int chatType, const QDateTime& time, const QVariant& data);

    /**
     * @brief 构建一个对方发送过来的聊天控件
     * @param chatType 聊天类型 @see IChatItem::ChatType
     * @param uid 与我聊天的用户id
     * @param time 接收时间
     * @param data 接收的数据
     * @return 返回创建好的聊天类，如果创建失败则返回nullptr
     * @note 注意，构建好的聊天数据不会保存到数据库
     */
    static IChatItem* BuildChatItem(int chatType, unsigned int uid, const QDateTime& time, const QVariant& data);

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
    Q_INVOKABLE void clearChatRecord();

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
     * @brief 发送一条文本聊天消息，会改变数据库的内容
     * @param msg 普通文本消息
     * @note 该方法是最常用来发送普通消息或富文本
     */
    Q_INVOKABLE inline void sendChat(const QString& msg)
    {
        sendChat(IChatItem::Text, msg);
    }

    /**
     * @brief 发送一条聊天消息，会改变数据库的内容
     * @param type 消息类型
     * @param data 消息数据
     * @note 该方法为通用方法，可以发送普通文本、富文本、图片和文件等控件
     */
    Q_INVOKABLE void sendChat(int chatType, const QVariant& data);

    /**
     * @brief 获取当前聊天视图的聊天对象
     * @return IChatObject* 返回聊天对象的指针
     * @note 如果不使用void load(IChatObject* chatObj);方法加载视图，则返回结果为空。即
     * 设置聊天对象方法应当使用load替代，那么一旦设置后无法更改了。
     * @see load()
     */
    inline IChatObject* getChatObject(void) const
    {
        return mChatObject.data();
    }

    /**
     * @brief 直接在尾部追加一条消息，不会改变数据库的内容
     * @param chat 要追加的聊天消息
     * @note 该方法通常作为接收消息使用，每调用一次就发射 @see chatAdded 信号
     */
    inline void appendChat(IChatItem* chat)
    {
        insertChat(mChats.size(), chat);
        emit chatAdded(chat->mChatObject->getRoleType() == IChatObject::Me);
    }

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    inline QHash<int, QByteArray> roleNames() const override
    {
        mRegistryChatClasses.insert(0, QByteArrayLiteral("chatItem"));
        return mRegistryChatClasses;
    }

Q_SIGNALS:
    void failed(const QString& msg);

    /**
     * @brief 信号：用户没有找到，即不是我的好友发送的消息
     * @param item 聊天信息
     */
    //void userMissed(IChatItem* item);

    /** 
     * @brief 信号：当聊天消息增加时
     * @param 是否是我发出的消息
     */
    void chatAdded(bool isMe);

protected Q_SLOTS:
    /**
     * @brief 加载更多
     * @param parent 可以忽视该参数
     */
    void fetchMore(const QModelIndex& parent = QModelIndex()) override;

    /**
     * @brief 接收一条消息，并会将正确接收下的聊天消息保存到数据库中
     * @param item 需要接收的消息项，由于sourceData的参数选择，可能会将该item抛弃
     * @param sourceData 发送来的可用数据。可用作筛选参数作为使用
     */
    void onReceived(IChatItem* item, const QVariantMap& sourceData);

    /**
     * @brief c初始化加载与给定用户的聊天消息
     * @param chatObj 聊天对象
     * @note 一般来说只用一次
     */
    void initLoad(IChatObject* chatObj);

private:
    /** 所有的聊天消息容器 */
    QList<QPointer<IChatItem>> mChats;

    /** 指向当前聊天对象的指针 */
    QPointer<IChatObject> mChatObject;

    /**
     * @brief 注册的聊天类容器
     * @type int 表示类的 ChatType
     * @type QByteArray 表示类的名称，即在qml中可以访问的类名
     */
    static QHash<int, QByteArray> mRegistryChatClasses;
};

#endif // CHATVIEW_H
