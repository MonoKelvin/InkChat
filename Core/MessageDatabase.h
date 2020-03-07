#ifndef MESSAGEDATABASE_H
#define MESSAGEDATABASE_H

#include <IChatObject.h>
#include <QSharedPointer>
#include <QSqlDatabase>

class IChatItem;
class ChatView;
class MessageItem;
class MessageList;

/**
 * @brief 消息数据库类
 * @note 主要完成消息的数据库管理，对消息增删查
 * TODO 把部分方法返回值改为void，错误消息使用变量QSqlError保存
 */
class MessageDatabase : public QObject {

    Q_OBJECT

public:
    ~MessageDatabase();

    QSqlError initDatabase();

    static inline QSharedPointer<MessageDatabase> Instance()
    {
        static QSharedPointer<MessageDatabase> instance(new MessageDatabase);
        return instance;
    }

    // 关闭数据库
    inline void close(void)
    {
        if (mDatabase.isOpen()) {
            mDatabase.close();
        }
    }

    //    MessageItem* queryMessage(const QString& str, const QDateTime& time);

    /**
     * @brief 加载消息到列表中
     * @param list 消息列表 @see MessageList
     * @return bool 加载成功返回true，否则返回false
     */
    bool loadMessageItems(MessageList* list);

    /**
     * @brief 加载的局域网消息到列表中
     * @param list 消息列表 @see MessageList
     * @param hostAddress 局域网地址，如果为空则默认加载所有本地保存的局域网消息
     * @return bool 加载成功返回true，否则返回false
     */
    bool loadLanMessageItems(MessageList* list, const QString& hostAddress = QString());

    /**
     * @brief 更新阅读消息标记
     * @param item 消息项
     * @return 更新成功返回true，否则返回false
     */
    bool updateReadFlag(MessageItem* item);

    /**
     * @brief 更新未读消息数量
     * @param item 消息项
     * @return 更新成功返回true，否则返回false
     */
    bool updateUnreadMsgCount(MessageItem* item);

    /**
     * @brief 加载聊天消息到聊天视图中
     * @param chatView 聊天视图 @see ChatView
     * @param uid 聊天对象ID
     * @return 加载成功返回true，否则返回false
     * @note 每次加载指定数量的聊天消息，最大为 @see CHAT_MESSAGE_FECTH_COUNT。加载时不
     * 会覆盖已加载的消息。
     */
    bool loadChatMessages(ChatView* chatView, unsigned int uid);

    /**
     * @brief 保存一条聊天记录
     * @param chatObjId 和我聊天的对象（单个用户或群聊）的id
     * @param item 消息
     * @return 保存成功返回true，并且将成功后的消息id保存到item中，保存失败返回false
     */
    bool saveAChatRecord(unsigned int chatObjId, IChatItem* item);

private:
    Q_DISABLE_COPY_MOVE(MessageDatabase)
    MessageDatabase();

    QSqlDatabase mDatabase;
};

#endif // MESSAGEDATABASE_H
