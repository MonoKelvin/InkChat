﻿#ifndef MESSAGEDATABASE_H
#define MESSAGEDATABASE_H

#include <IChatObject.h>

#include <QSharedPointer>
#include <QSqlDatabase>

class ChatItem;
class ChatList;
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
     * @brief 加载消息项。如果给定的聊天对象没有缓存则再数据库建立索引
     * @param list 消息列表 @see MessageList
     * @param chatObj 消息对应的聊天对象
     * @return MessageItem* 如果成功建立并保存到数据库中就返回MessageItem对象，且自动加
     * 载到列表中，否则返回nullptr
     */
    MessageItem* loadMessageItem(MessageList* list, IChatObject* chatObj);

    /**
     * @brief 刷新消息列表
     * @param list 消息列表 @see MessageList
     */
    void refreshMessageList(MessageList* list);

    /**
     * @brief 获取缓存的局域网对象
     * @param md5_16 16为的md5，其格式为(hostAddr + msc)再取中间16位
     * @return 返回局域网uuid，如果不存在则返回空字符串
     */
    LanObject* getCachedLanObject(const QString& md5_16);

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
     * @brief 更新置顶
     * @param item 消息项
     * @return 更新成功返回true，否则返回false
     */
    bool updateTop(MessageItem* item);

    /**
     * @brief 加载聊天消息到聊天视图中
     * @param chatView 聊天视图 @see ChatView
     * @return 加载成功返回true，否则返回false
     * @note 每次加载指定数量的聊天消息，最大为 @see CHAT_MESSAGE_FECTH_COUNT。加载时不
     * 会覆盖已加载的消息。
     */
    bool loadChatItems(ChatList* chatView);

    /**
     * @brief 保存一条聊天记录
     * @param item 发送者发送的消息
     * @param chatObjUuid 聊天对象的Uuid
     * @return 保存成功返回true，并且将成功后的消息(chatId)保存到item中，保存失败返回false
     */
    bool saveAChatRecord(ChatItem* item, const QString& chatObjUuid);

    const QSqlDatabase& getDatabase(void) { return mDatabase; }

    void clearChatRecord(const QString& uuid) noexcept;

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

private:
    QSqlDatabase mDatabase;
};

#endif // MESSAGEDATABASE_H
