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
const QString SqlQueryMsgItems = QStringLiteral("select uid,roleType,unreadMsgCount,readFlag,time,data from GetMsgItems");
// 查询与用户id的聊天记录
const QString SqlQueryChatById = QStringLiteral("select id,type,isMe,time,data from chatrecord where uid=%1 order by id desc limit %2,%3");
// 插入一条通知消息
const QString SqlInsertMessageItem = QStringLiteral("insert into message values(?,?,?,?,?,?)");
// 插入一条聊天记录
const QString SqlInsertChatRecord = QStringLiteral("insert into chatrecord (uid,type,isMe,time,data) values (?,?,?,?,?)");
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
            uid integer unsigned primary key,
            chat boolean defult '0',
            roleType smallint,
            unreadMsgCount integer default 1,
            readFlag boolean default 1
        ))");

    // 聊天记录表
    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id integer primary key autoincrement,
            uid integer unsigned,
            type integer default 1,
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
    copyFile(mDatabase.databaseName(), lanDbFile);

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
        switch (roleType & IChatObject::AllUser) {
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

bool MessageDatabase::loadLanMessageItems(MessageList* list)
{
    selectDatabaseFile(IChatObject::LAN);

    QSqlQuery query;
    LanObject* lan = LanObject::DetectLanEnvironment();
    //qDebug() << lan->getHostAddress() << lan->getMacAddress();

    // 加载实时连接的局域网
    if (nullptr != lan) {
        MessageItem* item = nullptr;

        // 如果有检测到局域网并且未被缓存，即没有分配id，则分配一个id，再保存到数据库
        if (lan->getID() == 0) {
            const QString SqlGetMaxId = QStringLiteral("select max(uid) from message");
            if (!query.exec(SqlGetMaxId)) {
                delete lan;
                lan = nullptr;
                emit list->failed(tr("无法加载局域网消息"));
                return false;
            }
            lan->setID(query.value(0).toUInt() + 1);

            // 设置索引文件
            {
                QSettings index(AppSettings::LanIndexFile(), QSettings::IniFormat);
                index.setValue(QString::number(lan->getID()), encryptTextByMD5(lan->getHostAddress() + lan->getMacAddress()));
            }

            if (query.prepare(SqlInsertMessageItem)) {
                query.addBindValue(lan->getID());
                query.addBindValue(IChatObject::LAN);
                query.addBindValue(0);
                query.addBindValue(true);
                query.addBindValue("");
                query.addBindValue("");
                query.exec();
            } else {
                emit list->failed(tr("通知消息未能成功保存到数据库，VALUE=") + QString::number(lan->getID()));
            }
        }

        User::Instance()->addChatObject(lan);

        item = new MessageItem;
        item->setChatObject(lan);
        list->appendMessage(item);
    }

    // 加载缓存的局域网对象
    if (!query.exec(SqlQueryMsgItems)) {
        return false;
    }
    while (query.next()) {
        MessageItem* item = new MessageItem;

        const auto lanId = query.value(0).toUInt();

        item->setChatObject(User::Instance()->getLanObjectById(lanId));
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
    if (q.exec(SqlUpdateReadFlag.arg(item->mUnreadMsgCount).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::loadChatItems(ChatView* chatView, IChatObject* chatObj)
{
    selectDatabaseFile(chatObj->getRoleType());

    QSqlQuery q;
    if (!q.exec(SqlQueryChatById.arg(chatObj->getID()).arg(chatView->mChats.size()).arg(CHAT_RECORD_FECTH_COUNT))) {
        qDebug() << q.lastError();
        return false;
    }

    int type = 0;
    while (q.next()) {
        // 聊天类型
        type = q.value(1).toInt();

        IChatItem* item = chatView->BuildChatItem(type, q.value(2).toBool(), chatObj->getID());
        if (nullptr == item) {
            return false;
        }

        item->mChatId = q.value(0).toInt();
        item->mTime = q.value(3).toDateTime();
        item->praseData(q.value(4));

        chatView->insertChat(0, item);
    }

    return true;
}

bool MessageDatabase::saveAChatRecord(IChatItem* item)
{
    QSqlQuery query;
    if (!query.prepare(SqlInsertChatRecord)) {
        return false;
    }

    query.addBindValue(item->mChatObject->getID());
    query.addBindValue(item->getChatType());
    query.addBindValue((item->mChatObject->getRoleType() == IChatObject::Me));
    query.addBindValue(item->mTime);
    query.addBindValue(item->getData());

    if (!query.exec()) {
        return false;
    }

    item->mChatId = query.lastInsertId().toInt();
    return true;
}
