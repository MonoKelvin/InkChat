#include "MessageDatabase.h"

#include <AppSettings.h>
#include <ChatView.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MyFriend.h>
#include <User.h>
#include <Utility.h>

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>

const auto SqlQueryMessage = QStringLiteral("select uid,roleType,lastMsg,lastTime,unreadMsgCount,readFlag from message where chat='1'");
const auto SqlInsertMessage = QStringLiteral("insert into message (uid,chat,roleType,lastMsg,lastTime,unreadMsgCount,readFlag) values (?,?,?,?,?,?,?)");

const auto SqlQueryChatById = QStringLiteral("select id,type,isMe,time,data from chatrecord where uid=%1 order by id desc limit %2,%3");
const auto SqlInsertChatRecord = QStringLiteral("insert into chatrecord (id,uid,type,isMe,time,data) values (null,?,?,?,?,?)");

const auto SqlUpdateReadFlag = QStringLiteral("update message set readFlag=%1 where uid=%2");
const auto SqlUpdateUnreadMsgCount = QStringLiteral("update message set unreadMsgCount=%1 where uid=%2");

MessageDatabase::MessageDatabase()
{
}

MessageDatabase::~MessageDatabase()
{
    close();

    qDebug() << "MessageDatabase Destroyed";
}

QSqlError MessageDatabase::initDatabase()
{
    mDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    mDatabase.setDatabaseName(AppSettings::MessageCacheFile());

    if (!mDatabase.open()) {
        return mDatabase.lastError();
    }

    const QStringList tables = mDatabase.tables();
    if (tables.contains(QStringLiteral("message"), Qt::CaseInsensitive)
        || tables.contains(QStringLiteral("chatrecord"), Qt::CaseInsensitive)) {
        return QSqlError();
    }

    const auto SqlCreateMessageItemTable = QLatin1String(R"(
        create table message(
            uid integer unsigned primary key,
            chat boolean defult 0,
            roleType smallint,
            lastMsg varchar,
            lastTime datetime,
            unreadMsgCount integer default 1,
            readFlag boolean default 1
        ))");

    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id integer primary key autoincrement,
            uid integer unsigned,
            type integer default 1,
            isMe boolean,
            time datetime,
            data text
        ))");

    QSqlQuery query;
    if (!query.exec(SqlCreateMessageItemTable)) {
        return query.lastError();
    }
    if (!query.exec(SqlCreateChatRecordTable)) {
        return query.lastError();
    }
    return QSqlError();
}

bool MessageDatabase::loadMessageItems(MessageList* list)
{
    QSqlQuery query;
    if (!query.exec(SqlQueryMessage)) {
        return false;
    }

    while (query.next()) {
        MessageItem* item = new MessageItem;

        const auto roleType = query.value(1).toInt();
        const auto chatObjId = query.value(0).toUInt();

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

        item->mMessage = query.value(2).toString();
        item->mTime = GetMessageTime(query.value(3).toDateTime());
        item->mUnreadMsgCount = query.value(4).toInt();
        item->mReadFlag = query.value(5).toBool();

        list->appendMessage(item);
    }

    return true;
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
    if (q.exec(SqlUpdateReadFlag.arg(item->mUnreadMsgCount).arg(item->mChatObject->getID()))) {
        return true;
    }

    qDebug() << q.lastError();
    return false;
}

bool MessageDatabase::loadChatItems(ChatView* chatView, unsigned int uid)
{
    QSqlQuery q;
    if (!q.exec(SqlQueryChatById.arg(uid).arg(chatView->mChats.size()).arg(CHAT_RECORD_FECTH_COUNT))) {
        qDebug() << q.lastError();
        return false;
    }

    int type = 0;
    while (q.next()) {
        // 聊天类型
        type = q.value(1).toInt();

        IChatItem* item = chatView->BuildChatItem(type, q.value(2).toBool(), uid);
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
