#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <ChatView.h>
#include <MessageItem.h>
#include <MessageList.h>

#include <QSqlDatabase>
#include <Singleton.h>

#define MESSAGEPAGE_INITIALIZA \
    CHATVIEW_INITIALIZA        \
    MESSAGEITEM_INITIALIZA     \
    MESSAGELIST_INITIALIZA

/**
 * @brief 消息数据库类
 * @note 主要完成消息的数据库管理，对消息增删查
 */
class MessageDatabase : public Singleton<MessageDatabase> {
    Q_DISABLE_COPY_MOVE(MessageDatabase)
public:
    MessageDatabase();
    ~MessageDatabase();

    QSqlError initDatabase(const QString& fileName);

    static inline MessageDatabase& Instance()
    {
        static MessageDatabase instance;
        return instance;
    }

    // 创建数据库并建立连接
    bool open(void);

    // 关闭数据库
    void close(void);

    // 创建数据表（student）
    void createTable(void);

    // 在表格中增加新的字段
    void addNewcolumn(QString& columnNameAndproperty);

    // 查询和显示结果
    void query(QString& str);

    // 判断数据表是否存在
    bool tableExists(QString& Tabname);

    // 删除数据
    void deletedata();

    // 修改数据
    void updatedata();

private:
    QSqlDatabase mDatabase;
};

#endif //MESSAGEPAGE_H
