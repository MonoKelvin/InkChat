#include "FriendPage.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <MyFriend.h>
#include <User.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FriendPage::FriendPage(QObject *parent) : QObject(parent)
{
}

FriendPage::~FriendPage()
{
}

void FriendPage::requestFriendsData(User* user)
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
                    MyFriend* myFriend = new MyFriend(user);
                    myFriend->fromJson(frd.toObject());
                    user->addFriend(myFriend);
                }
            } else {
                emit failed(tr("好友数据获取失败"));
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc, failed);
        }
    });
}
