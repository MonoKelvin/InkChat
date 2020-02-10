#include "User.h"

#include <InkChatApi.h>
#include <MyFriend.h>
#include <Utility.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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
    mPassword = json.value(QStringLiteral("password")).toString();

    QJsonArray friends = json.value(QStringLiteral("friends")).toArray();
    for (const auto i : friends) {
        MyFriend* f = new MyFriend(this);
        f->fromJson(i.toObject());
        addFriend(f);
    }

    return IPerson::fromJson(json);
}

QJsonObject User::toJson()
{
    auto json = IPerson::toJson();

    json.insert(QStringLiteral("account"), QJsonValue(mAccount));
    json.insert(QStringLiteral("password"), QJsonValue(mPassword));

    QJsonArray friends;
    for (const auto i : mFriends) {
        friends.append(i->toJson());
    }
    json.insert(QStringLiteral("friends"), friends);

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

bool User::cacheData()
{
    const auto fileName = USER_DATA_FILE(mID);
    if (isFileExists(fileName, true)) {
        std::thread writeFileThread([this, fileName] {
            QFile file(fileName);
            try {
                if (!file.open(QFile::WriteOnly | QFile::Text)) {
                    throw tr("用户数据文件打开失败！");
                }

                // TODO: ARS加密文件、MD5生成文件名

                QJsonDocument jsonDoc(toJson());
                if (file.write(jsonDoc.toJson(QJsonDocument::Indented /*Compact*/)) != -1) {
                    throw tr("用户数据文件写入失败！");
                }
            } catch (const QString& msg) {
                file.close();
                // 暂停1s，防止主页面未加载完就显示错误提示
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                emit failed(msg);
            }
        });

        writeFileThread.detach();
    } else {
        emit failed(tr("数据缓存失败！"));
        return false;
    }

    return true;
}

#include <QSettings>
bool User::loadData()
{
    const auto fileName = USER_DATA_FILE(mID);
    // 如果有缓存就加载缓存数据，否则从数据库中获取
    if (isFileExists(fileName)) {
        QFile file(fileName);
        try {
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                throw "读取用户缓存文件失败";
            }

            // TODO: 解密、校验数据是否被篡改

            auto jsonDoc = QJsonDocument::fromJson(file.readAll());
            if (jsonDoc.isNull() || !jsonDoc.isObject()) {
                throw "解析用户缓存数据失败";
            } else {
                fromJson(jsonDoc.object());
                return true;
            }
        } catch (const char* msg) {
            file.close();
            // TODO: 日志
            qDebug() << msg;
        }
    }

    return false;
}
