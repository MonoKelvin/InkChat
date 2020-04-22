#include "MessageDatabase.h"

#include <AppSettings.h>
#include <ChatList.h>
#include <Configuation.h>
#include <LanObject.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MyFriend.h>
#include <User.h>

#include <QDateTime>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

// 查询消息
const QString SqlQueryMsgItems = QStringLiteral("select * from getmessageitems");
// 查询用户的聊天记录
const QString SqlQueryChatRecord = QStringLiteral("select id,type,isMe,time,data from chatrecord where msgId=%1 order by id desc limit %2,%3");
// 插入一条通知消息
const QString SqlInsertMessageItem = QStringLiteral("insert into message values (?,?,?,?,?,?)");
// 插入一条聊天记录
const QString SqlInsertChatRecord = QStringLiteral("insert into chatrecord (id,msgId,type,isMe,time,data) values (?,?,?,?,?,?)");
// 更新阅读标志
const QString SqlUpdateReadFlag = QStringLiteral("update message set readFlag=%1 where uid=%2");
// 更新未读消息数目
const QString SqlUpdateUnreadMsgCount = QStringLiteral("update message set unreadMsgCount=%1 where uid=%2");

MessageDatabase::MessageDatabase()
{
}

MessageDatabase::~MessageDatabase()
{
    close();
    qDebug() << "MessageDatabase Destroyed: " << this;
}

QSqlError MessageDatabase::initDatabase()
{
    mDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    mDatabase.setDatabaseName(AppSettings::MessageDBFile());
    if (mDatabase.password().isEmpty()) {
        mDatabase.setUserName(User::Instance()->getNickName());
        mDatabase.setPassword(User::Instance()->getPassword());
    }

    if (!mDatabase.open(User::Instance()->getNickName(), User::Instance()->getPassword())) {
        return mDatabase.lastError();
    }

    const QStringList tables = mDatabase.tables();
    if (tables.contains(QStringLiteral("message"), Qt::CaseInsensitive)
        || tables.contains(QStringLiteral("chatrecord"), Qt::CaseInsensitive)) {
        return QSqlError();
    }

    // 消息表
    const auto SqlCreateMessageItemTable = QLatin1String(R"(
        create table message(
            id integer primary key autoincrement,
            chat boolean defult '0',
            uuid char(32) not null,
            unreadMsgCount int default 1,
            readFlag boolean default 1,
            top boolean default '0',
            md5 varchar(16) not null
        ))");

    // 聊天记录表
    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id integer primary key autoincrement,
            msgId integer,
            type int,
            isMe boolean,
            time datetime,
            data text,
            foreign key (msgId) references message (id) on delete cascade
        ))");

    // 消息视图
    const auto SqlCreateViewGetMsgItems = QLatin1String(R"(
            create view getmessageitems as select * from (
            select m.id,roleType,unreadMsgCount,readFlag,time,`data`,top
            from message as m left outer join chatrecord as c
            on m.id=c.msgId and m.chat='1'
            and c.id=(select max(id) from chatrecord where msgId=m.id)) as x
        )");

    QSqlQuery query;
    if (!query.exec(SqlCreateMessageItemTable)) {
        qDebug() << query.lastError();
        return query.lastError();
    }
    if (!query.exec(SqlCreateChatRecordTable)) {
        qDebug() << query.lastError();
        return query.lastError();
    }
    if (!query.exec(SqlCreateViewGetMsgItems)) {
        qDebug() << query.lastError();
        return query.lastError();
    }

    return QSqlError();
}

void MessageDatabase::loadMessageItems(MessageList* list)
{
    QSqlQuery query;
    if (!query.exec(SqlQueryMsgItems)) {
        emit list->failed(query.lastError().text());
        return;
    }
    while (query.next()) {
        const auto& obj = User::Instance()->getChatObjectByUuid(query.value(1).toString());

        // 获取数据失败
        if (nullptr == obj) {
            continue;
        }

        MessageItem* item = new MessageItem;

        item->setChatObject(obj);
        item->mUnreadMsgCount = query.value(2).toInt();
        item->mReadFlag = query.value(3).toBool();
        item->mTime = GetMessageTime(query.value(4).toDateTime());
        item->mMessage = query.value(5).toString();
        item->setIsTop(query.value(6).toBool());

        list->appendMessage(item);
    }

    // WARNING: 必须在加载完所有缓存数据后才能调用
    detectLanEnvironment(list);
}

LanObject* MessageDatabase::getCachedLanObject(const QString& md5_16)
{
    QSqlQuery query;
    if (query.exec(QStringLiteral("select uuid from message where md5=") + md5_16)) {
        while (query.next()) {
            return User::Instance()->getLanObjectByUuid(query.value(0).toString());
        }
    }

    return nullptr;
}

void MessageDatabase::detectLanEnvironment(MessageList* list)
{
    LanObject* lan = LanObject::DetectLanEnvironment();

    // 加载实时连接的局域网
    if (nullptr != lan) {
        // 如果该局域网对象已经在聊天列表中就直接返回
        if (list->isChatObjectExists(lan)) {
            return;
        }

        // 如果有检测到局域网并且未被缓存，即没有分配id，则分配一个id，再保存到数据库
        if (lan->getID() == 0) {
            QSqlQuery query;
            if (query.exec(QStringLiteral("select max(id) from message"))) {
                query.next();
                lan->setID(query.value(0).toUInt() + 1);
                lan->updateLocalData();
            } else {
                // 释放内存
                delete lan;
                lan = nullptr;

                emit list->failed(tr("无法存储新的聊天数据，请尝试清理缓存！"));
                return;
            }

            query.prepare(SqlInsertMessageItem);
            query.addBindValue(lan->getID());
            query.addBindValue(true);
            query.addBindValue(lan->getUuid());
            query.addBindValue(0);
            query.addBindValue(true);
            query.addBindValue(false);
            query.addBindValue(lan->getMD5());
            if (!query.exec()) {
                emit list->failed(tr("数据库未能成功建立消息记录，IP=") + lan->getHostAddress());
            }
        }

        MessageItem* item = new MessageItem;
        item->setChatObject(lan);

        list->appendMessage(item);
    }
}

bool MessageDatabase::updateReadFlag(MessageItem* item)
{
    QSqlQuery q;
    if (q.exec(SqlUpdateReadFlag.arg(item->mReadFlag).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::updateUnreadMsgCount(MessageItem* item)
{
    QSqlQuery q;
    if (q.exec(SqlUpdateUnreadMsgCount.arg(item->mUnreadMsgCount).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::loadChatItems(ChatList* chatView)
{
    const auto& chatObj = chatView->getChatObject();
    Q_ASSERT(chatObj != nullptr);

    QSqlQuery query;

    if (!query.exec(SqlQueryChatRecord.arg(chatObj->getID()).arg(chatView->mChats.size()).arg(CHAT_RECORD_FECTH_COUNT))) {
        return false;
    }

    int type = 0;
    while (query.next()) {
        // 聊天类型
        type = query.value(1).toInt();

        ChatItem* item = chatView->BuildChatItem(type, query.value(2).toBool(), chatObj->getID());
        if (nullptr == item) {
            continue;
        }

        item->mChatId = query.value(0).toInt();
        item->mTime = query.value(3).toDateTime();
        item->praseData(query.value(4));

        chatView->insertChat(0, item);
    }

    return true;
}

bool MessageDatabase::saveAChatRecord(ChatItem* item, IChatObject* chatObj)
{
    QSqlQuery query;
    if (!query.prepare(SqlInsertChatRecord)) {
        return false;
    }

    // id,msgid,type,isMe,time,data
    query.addBindValue(item->mChatObject->getID());
    query.addBindValue(chatObj->getID());
    query.addBindValue(item->getChatType());
    query.addBindValue((item->mChatObject->getRoleType() == IChatObject::Me));
    query.addBindValue(item->mTime);
    query.addBindValue(item->getData());

    if (query.exec()) {
        item->mChatId = query.lastInsertId().toInt();
        return true;
    }

    return false;
}
