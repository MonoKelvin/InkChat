#ifndef MESSAGEDATABASE_H
#define MESSAGEDATABASE_H

#include <IChatObject.h>
#include <QSharedPointer>
#include <QSqlDatabase>

class IChatItem;
class ChatView;
class MessageItem;
class MessageList;
class LanObject;

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
     * @note 任何错误将通过list的failed信号发出
     */
    void loadLanMessageItems(MessageList* list);

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
     * @return 加载成功返回true，否则返回false
     * @note 每次加载指定数量的聊天消息，最大为 @see CHAT_MESSAGE_FECTH_COUNT。加载时不
     * 会覆盖已加载的消息。
     */
    bool loadChatItems(ChatView* chatView);

    /**
     * @brief 保存一条聊天记录
     * @param item 消息
     * @param chatObj 聊天对象的实例
     * @return 保存成功返回true，并且将成功后的消息(chatId)保存到item中，保存失败返回false
     */
    bool saveAChatRecord(IChatItem* item, IChatObject* chatObj);

    QSqlDatabase& getDatabase(void) { return mDatabase; }

public Q_SLOTS:
    /**
     * @brief 检测局域网环境
     * @param list 检测到局域网后就添加到的消息列表
     * @note 如果缓存中已经有被检测到的局域网，则加载缓存数据（如果列表中已经加载则什么也不做）。
     * 否则保存到数据库中并添加到消息列表中，同时在局域网数据目录下添加该新局域网的索引。
     * @note 任何错误信息通过list的failed信号发出
     */
    void detectLanEnvironment(MessageList* list);

private:
    Q_DISABLE_COPY_MOVE(MessageDatabase)
    MessageDatabase();

    /**
     * @brief 选择数据库文件
     * @param type 根据角色类型选择文件
     */
    void selectDatabaseFile(IChatObject::ERoleType type);

private:
    QSqlDatabase mDatabase;
};

#endif // MESSAGEDATABASE_H
