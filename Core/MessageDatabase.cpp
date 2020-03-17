#include "MessageDatabase.h"

#include <AppSettings.h>
#include <ChatView.h>
#include <InkChatApi.h>
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
const QString SqlQueryMsgItems = QStringLiteral("select * from GetMsgItems");
// 查询与单用户的聊天记录
const QString SqlQueryChatsForSinglePerson = QStringLiteral("select id,type,isMe,time,data from chatrecord where uid=%1 order by id desc limit %2,%3");
// 查询多人聊天的聊天记录
const QString SqlQueryChatsForMultiPerson = QStringLiteral("select id,type,isMe,time,data from chatrecord where uid=%1 and msgId=%2 order by id desc limit %3,%4");
// 插入一条通知消息
const QString SqlInsertMessageItem = QStringLiteral("insert into message values (?,?,?,?,?)");
// 插入一条聊天记录
const QString SqlInsertChatRecord = QStringLiteral("insert into chatrecord (uid,msgid,type,isMe,time,data) values (?,?,?,?,?,?)");
// 更新阅读标志
const QString SqlUpdateReadFlag = QStringLiteral("update message set readFlag=%1 where uid=%2");
// 更新未读消息数目
const QString SqlUpdateUnreadMsgCount = QStringLiteral("update message set unreadMsgCount=%1 where uid=%2");

MessageDatabase::MessageDatabase()
{
}

void MessageDatabase::selectDatabaseFile(IChatObject::ERoleType type)
{
    const auto dbFile = AppSettings::MessageCacheFile(type);
    if (dbFile != mDatabase.databaseName()) {
        mDatabase.close();
        mDatabase.setDatabaseName(dbFile);
        mDatabase.open(User::Instance()->getNickName(), User::Instance()->getPassword());
    }
}

MessageDatabase::~MessageDatabase()
{
    close();
}

QSqlError MessageDatabase::initDatabase()
{
    mDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    mDatabase.setDatabaseName(AppSettings::MessageCacheFile());
    mDatabase.setUserName(User::Instance()->getNickName());
    mDatabase.setPassword(User::Instance()->getPassword());

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
            uid int unsigned primary key,
            chat boolean defult '0',
            roleType smallint,
            unreadMsgCount int default 1,
            readFlag boolean default 1
        ))");

    // 聊天记录表
    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id int primary key autoincrement,
            msgid int unsigned,
            uid int unsigned,
            type int default 1,
            isMe boolean,
            time datetime,
            data text
        ))");

    // 消息视图
    const auto SqlCreateViewGetMsgItems = QLatin1String(R"(
        create view GetMsgItems as
        select uid,roleType,unreadMsgCount,readFlag,time,data from (
            select m.uid,roleType,unreadMsgCount,readFlag,time,data,chat
            from message as m left outer join chatrecord as c
            on m.uid=c.uid and c.id = (select max(id) from chatrecord where uid=m.uid)
        ) as x where chat='1')");

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

    const auto lanDbFile = AppSettings::MessageCacheFile(IChatObject::LAN);
    copyFile(mDatabase.databaseName(), lanDbFile, false);

    return QSqlError();
}

bool MessageDatabase::loadMessageItems(MessageList* list)
{
    selectDatabaseFile(IChatObject::Friend);

    QSqlQuery query;
    if (!query.exec(SqlQueryMsgItems)) {
        return false;
    }

    while (query.next()) {
        MessageItem* item = new MessageItem;

        const auto roleType = query.value(1).toInt();
        const auto chatObjId = query.value(0).toUInt();

        // TODO: 添加更多的聊天对象
        switch (roleType) {
        case IChatObject::Friend:
            item->setChatObject(User::Instance()->getFriendById(chatObjId));
            break;
        default:
            break;
        }

        if (item->mChatObject.isNull()) {
            delete item;
            item = nullptr;
            return false;
        }

        item->mUnreadMsgCount = query.value(2).toInt();
        item->mReadFlag = query.value(3).toBool();
        item->mTime = GetMessageTime(query.value(4).toDateTime());
        item->mMessage = query.value(5).toString();

        list->appendMessage(item);
    }

    return true;
}

void MessageDatabase::loadLanMessageItems(MessageList* list)
{
    selectDatabaseFile(IChatObject::LAN);

    QSqlQuery query;
    if (!query.exec(SqlQueryMsgItems)) {
        emit list->failed(query.lastError().text());
        return;
    }
    while (query.next()) {
        MessageItem* item = new MessageItem;

        const auto lanId = query.value(0).toUInt();

        item->setChatObject(User::Instance()->getLanObjectById(lanId));
        if (item->mChatObject.isNull()) {
            delete item;
            item = nullptr;
            continue;
        }

        item->mUnreadMsgCount = query.value(2).toInt();
        item->mReadFlag = query.value(3).toBool();
        item->mTime = GetMessageTime(query.value(4).toDateTime());
        item->mMessage = query.value(5).toString();

        list->appendMessage(item);
    }

    // WARNING: 必须在加载完所有缓存数据后才能调用
    detectLanEnvironment(list);
}

bool MessageDatabase::updateReadFlag(MessageItem* item)
{
    selectDatabaseFile(item->mChatObject->getRoleType());

    QSqlQuery q;
    if (q.exec(SqlUpdateReadFlag.arg(item->mReadFlag).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::updateUnreadMsgCount(MessageItem* item)
{
    selectDatabaseFile(item->mChatObject->getRoleType());

    QSqlQuery q;
    if (q.exec(SqlUpdateUnreadMsgCount.arg(item->mUnreadMsgCount).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::loadChatItems(ChatView* chatView, IChatObject* chatObj)
{
    selectDatabaseFile(chatObj->getRoleType());

    QSqlQuery query;

    {
        QString sql = SqlQueryChatsForSinglePerson.arg(chatObj->getID());
        if (chatObj->getRoleType() & IChatObject::MultiPerson) {
            sql = sql.arg(chatObj->getID());
        } else {
            sql = sql.arg(chatView->mChats.size()).arg(CHAT_RECORD_FECTH_COUNT);
        }
        if (!query.exec(sql)) {
            return false;
        }
    }

    int type = 0;
    while (query.next()) {
        // 聊天类型
        type = query.value(1).toInt();

        IChatItem* item = chatView->BuildChatItem(type, query.value(2).toBool(), chatObj->getID());
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

bool MessageDatabase::saveAChatRecord(IChatItem* item, IChatObject* chatObj)
{
    selectDatabaseFile(chatObj->getRoleType());

    QSqlQuery query;
    if (!query.prepare(SqlInsertChatRecord)) {
        return false;
    }

    // uid,msgid,type,isMe,time,data
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
            selectDatabaseFile(IChatObject::LAN);

            QSqlQuery query;
            if (query.exec(QStringLiteral("select max(uid) from message"))) {
                query.next();
                lan->setID(query.value(0).toUInt() + 1);
            } else {
                // 释放内存
                delete lan;
                lan = nullptr;

                emit list->failed(tr("无法存储新的局域网数据，请尝试清理缓存！"));
                return;
            }

            // 添加索引
            {
                QSettings index(AppSettings::LanIndexFile(), QSettings::IniFormat);
                index.setValue(QString::number(lan->getID()), encryptTextByMD5(lan->getHostAddress() + lan->getMacAddress()));
            }

            query.prepare(SqlInsertMessageItem);
            query.addBindValue(lan->getID());
            query.addBindValue(true);
            query.addBindValue(IChatObject::LAN);
            query.addBindValue(0);
            query.addBindValue(true);
            if (!query.exec()) {
                emit list->failed(tr("数据库未能成功建立局域网记录，IP=") + lan->getHostAddress());
            }
        }

        MessageItem* item = new MessageItem;
        item->setChatObject(lan);

        list->appendMessage(item);
    }
}
