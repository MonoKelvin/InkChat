#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QAbstractListModel>

class IChatItem;

class ChatView : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ChatView)

    friend class MessageDatabase;

public:
    explicit ChatView(QObject* parent = nullptr);
    ~ChatView() override;

    IChatItem* buildChatItem(int role);

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
     * @brief 在指定位置插入一条聊天项
     * @param row 行号
     * @param chat 聊天项
     * @return bool 插入成功返回true，否则返回false
     * @warning 实际情况中尽量不要使用该函数，因为消息的增加几乎发生在从聊天视图的底部推送。
     * @see sendChat
     */
    bool insertMessage(int row, IChatItem* chat);

    /**
     * @brief 给定父元素下的行数量，即聊天总数
     * @param parent 指定的父元素
     * @return 行数
     */
    int rowCount(const QModelIndex& parent) const override;

public Q_SLOTS:
    /**
     * @brief 在发送一条聊天项
     * @param message 消息项
     * @return 发送成功返回true，否则返回false
     */
    bool sendChat(IChatItem* chat);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

private:
    /** 所有的聊天消息容器 */
    QList<IChatItem*> mChats;
};

#endif // CHATVIEW_H
