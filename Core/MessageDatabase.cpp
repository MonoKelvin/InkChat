#include "MessageDatabase.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <ChatList.h>
#include <Configuation.h>
#include <LanObject.h>
#include <MessageItem.h>
#include <MessageList.h>
#include <MessageManager.h>
#include <User.h>

#include <QSqlError>
#include <QSqlQuery>

// 查询消息
const QString SqlQueryMsgItems = QStringLiteral("select * from getmessageitems");
// 查询用户的聊天记录
const QString SqlQueryChatRecord = QStringLiteral("select type,uuid,time,data from chatrecord where msgId=(select id from message where uuid='%1') order by id desc limit %2,%3");
// 插入一条通知消息
const QString SqlInsertVoidMessageItem = QStringLiteral("insert into message (uuid,roleType,md5) values(?,?,?)");
// 插入一条聊天记录
const QString SqlInsertChatRecord = QStringLiteral("insert into chatrecord (msgId,type,uuid,time,data) values((select id from message where uuid=?),?,?,?,?)");
// 更新阅读标志
const QString SqlUpdateReadFlag = QStringLiteral("update message set readFlag=%1 where uuid='%2'");
// 更新未读消息数目
const QString SqlUpdateUnreadMsgCount = QStringLiteral("update message set unreadMsgCount=%1 where uuid='%2'");
// 更新置顶
const QString SqlUpdateTop = QStringLiteral("update message set top=%1 where uuid='%2'");

MessageDatabase::MessageDatabase()
{
}

MessageDatabase::~MessageDatabase()
{
    close();
    qDebug() << "MessageDatabase Destroyed: ";
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
            chat boolean default '1',
            uuid char(32) not null,
            roleType smallint,
            unreadMsgCount int default 0,
            readFlag boolean default '1',
            top boolean default '0',
            md5 char(16)
        ))");

    // 聊天记录表
    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id integer primary key autoincrement,
            msgId integer not null,
            type int,
            uuid char(32) not null,
            time datetime,
            data text,
            foreign key (msgId) references message (id) on delete cascade
        ))");

    // 消息视图
    const auto SqlCreateViewGetMsgItems = QLatin1String(R"(
            create view getmessageitems as select * from (
            select m.uuid as uuid,roleType,unreadMsgCount,readFlag,time,data,top
            from message as m left outer join chatrecord as c
            on c.id=(select max(id) from chatrecord where msgId=m.id)
            where chat='1') as x
        )");

    QSqlQuery query;
    if (!query.exec(SqlCreateMessageItemTable)) {
        qDebug() << query.lastError() << __LINE__;
        return query.lastError();
    }
    if (!query.exec(SqlCreateChatRecordTable)) {
        qDebug() << query.lastError() << __LINE__;
        return query.lastError();
    }
    if (!query.exec(SqlCreateViewGetMsgItems)) {
        qDebug() << query.lastError() << __LINE__;
        return query.lastError();
    }

    return QSqlError();
}

MessageItem* MessageDatabase::loadMessageItem(MessageList* list, IChatObject* chatObj)
{
    Q_ASSERT(nullptr != chatObj);

    // 如果列表中已经存在就直接返回
    {
        const int& i = list->getIndexByChatObject(chatObj);
        if (i >= 0) {
            return list->getMessage(i);
        }
    }

    QSqlQuery query;
    if (!query.exec(SqlQueryMsgItems + QStringLiteral(" where uuid='%1'").arg(chatObj->getUuid()))) {
        qDebug() << query.lastError() << __LINE__;
        return nullptr;
    }

    MessageItem* item = new MessageItem;
    item->mChatObject = chatObj;

    // 有缓存，直接加载
    if (query.next()) {
        // uuid,roleType,unreadMsgCount,readFlag,time,data,top
        item->mUnreadMsgCount = query.value(2).toInt();
        item->mReadFlag = query.value(3).toBool();
        item->mTime = GetMessageTime(query.value(4).toDateTime());
        item->mMessage = query.value(5).toString();
        item->mIsTop = query.value(6).toBool();
    } else {
        // uuid,roleType,md5
        query.prepare(SqlInsertVoidMessageItem);
        query.addBindValue(chatObj->getUuid());
        query.addBindValue(chatObj->getRoleType());
        query.addBindValue(chatObj->getMD5());

        if (query.exec()) {
            chatObj->updateLocalData();
            item->mMessage = tr("[暂无最近消息]");
        } else {
            qDebug() << query.lastError() << __LINE__;
            emit list->failed(tr("无法存储新的联系人信息，请尝试在[设置]中清理缓存！"));
            delete item;
            item = nullptr;
            return nullptr;
        }
    }

    list->appendMessage(item, true);
    return item;
}

void MessageDatabase::refreshMessageList(MessageList* list)
{
    QSqlQuery query;
    if (!query.exec(SqlQueryMsgItems)) {
        emit list->failed(query.lastError().text());
        qDebug() << query.lastError() << __LINE__;
        return;
    }
    while (query.next()) {
        // uuid,roleType,unreadMsgCount,readFlag,time,data,top
        const auto& obj = User::Instance()->getChatObjectByUuid(query.value(0).toString());

        // 获取数据失败
        if (!obj || list->getIndexByChatObject(obj) >= 0) {
            continue;
        }

        MessageItem* item = new MessageItem;

        item->setChatObject(obj);
        item->mUnreadMsgCount = query.value(2).toInt();
        item->mReadFlag = query.value(3).toBool();
        item->mTime = GetMessageTime(query.value(4).toDateTime());
        item->mMessage = query.value(5).toString();
        item->mIsTop = query.value(6).toBool();

        list->appendMessage(item);
    }

    detectLanEnvironment(list);
}

LanObject* MessageDatabase::getCachedLanObject(const QString& md5_16)
{
    QSqlQuery query;
    if (query.exec(QStringLiteral("select uuid from message where md5='%1'").arg(md5_16))) {
        while (query.next()) {
            return User::Instance()->getLanObjectByUuid(query.value(0).toString());
        }
    }

    return nullptr;
}

void MessageDatabase::detectLanEnvironment(MessageList* list)
{
    LanObject* lan = LanObject::DetectLanEnvironment();

    // 检测到局域网并且局域网未显示到视图
    if (lan && list->getIndexByChatObject(lan) == -1) {
        // 如果未被缓存，则分配一个uuid，再保存到数据库
        if (lan->getUuid().isEmpty()) {
            lan->generateUuid();
        }

        QSqlQuery query;
        query.prepare(SqlInsertVoidMessageItem);
        // uuid,roleType,md5
        query.addBindValue(lan->getUuid());
        query.addBindValue(IChatObject::LAN);
        query.addBindValue(lan->getMD5());
        if (query.exec()) {
            MessageItem* item = new MessageItem;
            item->mChatObject = lan;
            list->appendMessage(item);
            lan->updateLocalData();
        }
    }
}

bool MessageDatabase::updateReadFlag(MessageItem* item)
{
    QSqlQuery q;
    if (q.exec(SqlUpdateReadFlag.arg(item->mReadFlag).arg(item->mChatObject->getUuid()))) {
        return true;
    }

    qDebug() << q.lastError() << __LINE__;
    return false;
}

bool MessageDatabase::updateUnreadMsgCount(MessageItem* item)
{
    QSqlQuery q;
    if (q.exec(SqlUpdateUnreadMsgCount.arg(item->mUnreadMsgCount).arg(item->mChatObject->getUuid()))) {
        return true;
    }

    qDebug() << q.lastError() << __LINE__;
    return false;
}

bool MessageDatabase::updateTop(MessageItem* item)
{
    QSqlQuery q;
    if (q.exec(SqlUpdateTop.arg(item->mIsTop).arg(item->mChatObject->getUuid()))) {
        return true;
    }

    qDebug() << q.lastError() << __LINE__;
    return false;
}

bool MessageDatabase::loadChatItems(ChatList* chatView)
{
    const auto& chatObj = chatView->getChatObject();
    Q_ASSERT(chatObj != nullptr);

    QSqlQuery query;

    if (!query.exec(SqlQueryChatRecord.arg(chatObj->getUuid()).arg(chatView->mChats.size()).arg(CHAT_RECORD_FECTH_COUNT))) {
        emit chatView->failed(tr("消息获取失败，请再刷新一下"));
        return false;
    }

    while (query.next()) {
        ChatItem* item = nullptr;

        // type,uuid,time,data
        const QString& uuid = query.value(1).toString();
        if (uuid == User::Instance()->getUuid()) {
            item = MessageManager::BuildChatItem(query.value(0).toInt(), { uuid, User::Instance()->getNickName(), query.value(3) });
        } else {
            item = MessageManager::BuildChatItem(query.value(0).toInt(), { uuid, query.value(3) });
        }

        if (nullptr == item) {
            continue;
        }

        item->setTime(query.value(2).toDateTime());
        chatView->insertChat(0, item);
    }

    return true;
}

bool MessageDatabase::saveAChatRecord(ChatItem* item, const QString& chatObjUuid)
{
    QSqlQuery query;
    if (!query.prepare(SqlInsertChatRecord)) {
        qDebug() << query.lastError() << __LINE__;
        return false;
    }

    // msgId,type,uuid,time,data
    const auto& itemData = item->getChatItemData();
    query.addBindValue(chatObjUuid);
    query.addBindValue(item->getChatType());
    query.addBindValue(itemData.Uuid);
    query.addBindValue(item->getTime());
    query.addBindValue(itemData.Data);

    return query.exec();
}

void MessageDatabase::clearChatRecord(const QString& uuid) noexcept
{
    QSqlQuery query;
    query.exec(QStringLiteral("delete from chatrecord where msgId=(select id from message where uuid='%1')").arg(uuid));
}
