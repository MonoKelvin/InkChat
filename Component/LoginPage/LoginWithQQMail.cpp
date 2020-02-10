#include "LoginWithQQMail.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QQmlContext>

QPointer<QQmlApplicationEngine> LoginWithQQMail::QmlEngine = nullptr;
QSharedPointer<User> ILoginOperation::mUser = nullptr;

void LoginWithQQMail::loginRequest(const QVariantMap& mapping)
{
    HttpRequest* request = new HttpRequest;

    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    // 如果在本地数据加载用户成功，则进行ID验证；否则进行密码登录
    if (mUser->loadData()) {
        emit jumpLogin(mUser->mAccount, mUser->mPassword);

        QString postData = "id=" + QString::number(mUser->mID) + "&account=" + mUser->mAccount + "&password=" + mUser->mPassword;
        qDebug() << postData;
        request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);

        connect(request, &HttpRequest::request, [this](bool success, const QByteArray& jsonData) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (success) {
                if (jsonDoc.object().contains(QStringLiteral("success"))) {
                    emit verified();
                }
            } else {
                EMIT_FAILED_MESSAGE(jsonDoc, failed);
            }
        });
    } else {
        QString postData = "account=" + mapping["account"].toString() + "&password=" + mapping["password"].toString();
        request->sendRequest(LoginByPasswordUrl, HttpRequest::POST, postData);

        connect(request, &HttpRequest::request, [this, mapping](bool success, const QByteArray& jsonData) {
            QJsonParseError err;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
            if (success) {
                if (err.error == QJsonParseError::NoError) {
                    auto json = jsonDoc.object();

                    json.insert(QStringLiteral("account"), mapping["account"].toString());
                    json.insert(QStringLiteral("password"), mapping["password"].toString());

                    mUser->fromJson(json);
                    mUser->cacheData();
                    emit verified();
                } else {
                    emit failed(err.errorString());
                }
            } else {
                EMIT_FAILED_MESSAGE(jsonDoc, failed);
            }
        });
    }
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
            EMIT_FAILED_MESSAGE(jsonDoc, failed);
        }
    });
}

void LoginWithQQMail::redirect(QQmlApplicationEngine* engine, const QUrl& url)
{
    Q_UNUSED(engine)
    Q_ASSERT(QmlEngine != nullptr);

    connect(
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

void LoginWithQQMail::InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url)
{
    QmlEngine = qmlEngine;

    // connect(
    //     QmlEngine, &QQmlApplicationEngine::objectCreated, qApp,
    //     [url](QObject* obj, const QUrl& objUrl) {
    //         if (!obj && url == objUrl)
    //             QCoreApplication::exit(-1);
    //     },
    //     Qt::QueuedConnection);

    QmlEngine->load(url);
}
