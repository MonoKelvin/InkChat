﻿#include "User.h"

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

User::~User()
{
    qDeleteAll(mFriends);
    mFriends.clear();
}

QQmlListProperty<MyFriend> User::getFriends()
{
    return QQmlListProperty<MyFriend>(this, this->mFriends);
}

bool User::cacheUserData()
{
    auto fileName = USER_DATA_FILE(mUID);
    if (isFileExists(fileName, true)) {

        std::thread writeFileThread([this, fileName] {
            QFile file(fileName);
            file.open(QFile::WriteOnly | QFile::Text);

            QJsonObject jsonObj;

            jsonObj.insert("id", QJsonValue(QString::number(mUID)));
            jsonObj.insert("nickName", QJsonValue(mNickName));
            jsonObj.insert("signature", QJsonValue(mSignature));
            jsonObj.insert("account", QJsonValue(mAccount));
            jsonObj.insert("gender", QJsonValue(mGender));

            QJsonArray friends;
            for (const auto i : mFriends) {
                friends.append(i->toJsonObject());
            }
            jsonObj.insert("friends", friends);

            // TODO
            // jsonObj.insert("groups", QJsonArray());

            QJsonDocument jsonDoc(jsonObj);
            file.write(jsonDoc.toJson(QJsonDocument::Indented /*Compact*/));

            file.close();
        });

        writeFileThread.join();
    }

    return false;
}
