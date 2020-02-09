﻿#include "LoginWithQQMail.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <MyFriend.h>
#include <User.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QQmlContext>

#define EMIT_FAILED_MESSAGE(_JsonDoc_)                           \
    if (_JsonDoc_.object().contains("msg"))                      \
        emit failed(_JsonDoc_.object().value("msg").toString()); \
    else                                                         \
        emit failed(QStringLiteral("NO_RESPONSE_MESSAGE"));

QPointer<QQmlApplicationEngine> LoginWithQQMail::QmlEngine = nullptr;
QSharedPointer<User> ILoginOperation::mUser = nullptr;

LoginWithQQMail::LoginWithQQMail(const QUrl& url, const QString& contextName)
{
    Q_ASSERT(QmlEngine != nullptr);

    QObject::connect(
        QmlEngine,
        &QQmlApplicationEngine::objectCreated,
        qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    QmlEngine->rootContext()->setContextProperty(contextName, this);
    QmlEngine->load(url);
}

void LoginWithQQMail::loginRequest(const QVariantMap& mapping)
{
    HttpRequest* request = new HttpRequest;

    QString postData = "account=" + mapping["account"].toString()
        + "&password=" + mapping["password"].toString();

    request->sendRequest(LoginByPasswordUrl, HttpRequest::POST, postData);

    connect(request, &HttpRequest::request, [=](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                parse(jsonDoc.object().toVariantMap());
                emit verified();
            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc)
        }
    });
}

void LoginWithQQMail::signupRequest(const QVariantMap& mapping)
{
    HttpRequest* request = new HttpRequest;

    QString postData = "nickName=" + mapping["nickName"].toString() + "&account=" + mapping["account"].toString()
        + "&password=" + mapping["password"].toString();

    request->sendRequest(SignupUrl, HttpRequest::POST, postData);

    connect(request, &HttpRequest::request, [=](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                emit registered();
            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc)
        }
    });
}

void LoginWithQQMail::redirect(QQmlApplicationEngine* engine, const QUrl& url)
{
    Q_UNUSED(engine)
    Q_ASSERT(QmlEngine != nullptr);

    QObject::connect(
        QmlEngine, &QQmlApplicationEngine::objectCreated, qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    QmlEngine->rootContext()->setContextProperty(QStringLiteral("UserModel"), mUser.data());
    QmlEngine->load(url);
}

void LoginWithQQMail::parse(const QVariantMap& userJson)
{
    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    mUser->mUID = userJson["id"].toString().toUInt();
    mUser->mNickName = userJson["nickName"].toString();
    mUser->mAccount = USER_AVATAR_URL(mUser->mUID);
    mUser->mPassword = userJson["password"].toString();
    mUser->mAvatar = userJson["avatar"].toString();

    // 请求好友数据
    HttpRequest* request = new HttpRequest;
    request->sendRequest(LoginByPasswordUrl, HttpRequest::POST, QString("id=%1").arg(mUser->mUID));
    connect(request, &HttpRequest::request, [=](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {

                // TODO
                QJsonArray fArr = jsonDoc.array();
                for (const auto group : fArr) {
                    for (const auto frd : group.toObject()) {
                        MyFriend* myFriend = new MyFriend(mUser.data());
                        const auto obj = frd.toObject();
                        obj["subgroup"];
                    }
                }

                emit verified();
            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc)
        }
    });
}
