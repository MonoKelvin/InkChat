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

const auto SqlQueryChatById = QStringLiteral("select id,type,isMe,time,data from chatrecord where uid=%1 limit %2 offset %3");
const auto SqlInsertChatRecord = QStringLiteral("insert into chatrecord (uid,type,isMe,time,data) values (?,?,?,?,?)");

MessageDatabase::MessageDatabase()
{
}

MessageDatabase::~MessageDatabase()
{
    close();
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
            chat boolean,
            roleType smallint,
            lastMsg varchar,
            lastTime datetime,
            unreadMsgCount integer,
            readFlag boolean
        ))");

    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id integer primary key autoincrement,
            uid integer unsigned,
            type integer,
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

    /*if (query.prepare("insert into message (uid,chat,roleType,lastMsg,lastTime,unreadMsgCount,readFlag) values (?,?,?,?,?,?,?)")) {
        query.addBindValue(2);
        query.addBindValue(true);
        query.addBindValue(IChatObject::Friend);
        query.addBindValue("上一条消息，上一条消息上一条消息上一条消息，上一条消息");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(5);
        query.addBindValue(false);
        query.exec();

        query.addBindValue(3);
        query.addBindValue(true);
        query.addBindValue(IChatObject::Friend);
        query.addBindValue("上一条消息");
        query.addBindValue(QDateTime(QDate(2020, 2, 16)));
        query.addBindValue(200);
        query.addBindValue(true);
        query.exec();

        query.addBindValue(4);
        query.addBindValue(true);
        query.addBindValue(IChatObject::Friend);
        query.addBindValue("Hello! How are you?");
        query.addBindValue(QDateTime(QDate(2020, 1, 10)));
        query.addBindValue(1);
        query.addBindValue(false);
        query.exec();
    }*/

    if (!query.exec(QStringLiteral("select uid,roleType,lastMsg,lastTime,unreadMsgCount,readFlag from message where chat='1'"))) {
        return false;
    }

    while (query.next()) {
        MessageItem* item = new MessageItem;

        const auto roleType = query.value(1).toInt();
        switch (roleType & IChatObject::AllUser) {
        case IChatObject::Friend:
            item->mChatObject = QSharedPointer<IChatObject>(User::Instance()->getFriendById(query.value(0).toUInt()));
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

    list->adjustMessageOrder();

    return true;
}

bool MessageDatabase::loadChatMessages(ChatView* chatView, unsigned int uid)
{
    QSqlQuery q;
    if (!q.exec(SqlQueryChatById.arg(uid).arg(CHAT_MESSAGE_FECTH_COUNT).arg(chatView->rowCount(QModelIndex())))) {
        qDebug() << q.lastError();
        return false;
    }

    int type = 0;
    bool isMe = false;

    while (q.next()) {
        // 聊天类型
        type = q.value(1).toInt();

        IChatItem* item = chatView->buildChatItem(type, isMe, uid);
        if (nullptr == item) {
            return false;
        }

        item->mChatId = q.value(0).toInt();
        item->mTime = q.value(3).toDateTime();
        item->praseData(q.value(4));

        chatView->appendChat(item);
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
    query.addBindValue(item->ChatType);
    query.addBindValue((item->mChatObject->getRoleType() == IChatObject::Me));
    query.addBindValue(item->mTime);
    query.addBindValue(item->getData());

    if (!query.exec()) {
        return false;
    }

    item->mChatId = query.lastInsertId().toInt();
    return true;
}
