#include "MessagePage.h"

#include <AppSettings.h>
#include <User.h>

#include <QSqlError>
#include <QSqlQuery>

MessageDatabase::MessageDatabase()
{
    Instance();
}

MessageDatabase::~MessageDatabase()
{
    if (mDatabase.isOpen()) {
        mDatabase.close();
    }
}

QSqlError MessageDatabase::initDatabase(const QString& fileName)
{
    mDatabase = QSqlDatabase::addDatabase(QStringLiteral("MessageDB"));
    mDatabase.setUserName(User::Instance()->getAccount());
    mDatabase.setPassword(User::Instance()->getPassword());
    mDatabase.setDatabaseName(fileName);

    if (!mDatabase.open()) {
        return mDatabase.lastError();
    }

    const QStringList tables = mDatabase.tables();
    if (tables.contains(QStringLiteral("message"), Qt::CaseInsensitive)) {
        return QSqlError();
    }

    QSqlQuery q;

    const auto SqlCreateMessageItemTable = QLatin1String(R"(
        create table message(
            id unsigned integer primary key,
            chat boolean,
            roleType tinyint,
            lastMsg varchar,
            lastTime varchar(10),
            unreadMsgCount integer,
            readFlag boolean
        ))");

    const auto SqlCreateChatRecordTable = QLatin1String(R"(
        create table chatrecord(
            id unsigned integer primary key autoincrement,
            uid integer,
            is_me boolean,
            time datetime,
            msg_type tinyint,
            message text,
        ))");

    if (!q.exec(SqlCreateMessageItemTable)) {
        return q.lastError();
    }
    if (!q.exec(SqlCreateChatRecordTable)) {
        return q.lastError();
    }

    return QSqlError();
}
