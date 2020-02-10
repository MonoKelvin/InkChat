#include "MyFriend.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>
#include <Utility.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MyFriend::MyFriend(QObject* parent)
    : IPerson(parent)
{
}

void MyFriend::requestFriendsData(User* user)
{
    // 请求好友数据
    HttpRequest* request = new HttpRequest;
    request->sendRequest(MyFriendUrl, HttpRequest::POST, QString("id=%1").arg(user->getID()));
    connect(request, &HttpRequest::request, [this, user](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                // 解析好友数据
                for (const auto frd : jsonDoc.array()) {
                    try {
                        MyFriend* myFriend = new MyFriend(user);
                        myFriend->fromJson(frd.toObject());
                        user->addFriend(myFriend);
                    } catch (const QString&) {
                        emit failed(QStringLiteral("JSON_PARSE_FAILED"));
                        break;
                    }
                }
            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc, failed);
        }
    });
}

void MyFriend::fromJson(const QJsonObject& json)
{
    mIsTop = json.value("top").toBool();
    mRemark = json.value("remark").toString();
    mSubgroup = json.value("subgroup").toString();
    mHostAddress = json.value("hostAddress").toString();

    return IPerson::fromJson(json);
}

QJsonObject MyFriend::toJson()
{
    auto json = IPerson::toJson();

    json.insert("top", mIsTop);
    json.insert("remark", mRemark);
    json.insert("subgroup", mSubgroup);

    return json;
}
