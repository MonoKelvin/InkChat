#ifndef MESSAGEDATABASE_H
#define MESSAGEDATABASE_H

#include <QSharedPointer>
#include <QSqlDatabase>

class MessageItem;
class MessageList;

/**
 * @brief 消息数据库类
 * @note 主要完成消息的数据库管理，对消息增删查
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

private:
    Q_DISABLE_COPY_MOVE(MessageDatabase)
    MessageDatabase();

    QSqlDatabase mDatabase;
};

#endif // MESSAGEDATABASE_H
