#include "User.h"

#include <InkChatApi.h>
#include <MyFriend.h>
#include <Utility.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <thread>
using std::thread;

User::User(QObject* parent)
    : IPerson(parent)
{
}

User::~User()
{
    qDeleteAll(mFriends);
    mFriends.clear();
}

void User::fromJson(const QJsonObject& json)
{
    mAccount = json.value(QStringLiteral("account")).toString();

    return IPerson::fromJson(json);
}

QJsonObject User::toJson()
{
    auto json = IPerson::toJson();

    json.insert("account", QJsonValue(mAccount));

    QJsonArray friends;
    for (const auto i : mFriends) {
        friends.append(i->toJson());
    }
    json.insert("friends", friends);

    return json;
}

QQmlListProperty<MyFriend> User::getFriends()
{
    return QQmlListProperty<MyFriend>(this, this->mFriends);
}

void User::addFriend(MyFriend* myFriend)
{
    mFriends.append(myFriend);
}

void User::cacheData()
{
    auto fileName = USER_DATA_FILE(mID);
    if (isFileExists(fileName, true)) {
        std::thread writeFileThread([this, fileName] {
            QFile file(fileName);
            try {
                if (!file.open(QFile::WriteOnly | QFile::Text)) {
                    throw tr("用户数据文件打开失败！");
                }

                // TODO
                // jsonObj.insert("groups", QJsonArray());

                QJsonDocument jsonDoc(toJson());
                if (file.write(jsonDoc.toJson(QJsonDocument::Indented /*Compact*/)) == -1) {
                    throw tr("用户数据文件写入失败！");
                }
            } catch (const QString& msg) {
                file.close();
                emit failed(msg);
            }
        });

        writeFileThread.join();
    } else {
        emit failed(tr("数据缓存失败！"));
    }
}
