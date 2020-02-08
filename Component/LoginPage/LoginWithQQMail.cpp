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

    QString postData = "verify_method=password&account=" + mapping["account"].toString()
        + "&password=" + mapping["password"].toString();

    request->sendRequest(UserLoginUrl, HttpRequest::POST, postData);

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
            emit jsonDoc.object().contains("msg") ? failed(jsonDoc.object().value("msg").toString())
                                                  : failed(QStringLiteral("NO_RESPONSE_MESSAGE"));
        }
    });
}

void LoginWithQQMail::signupRequest(const QVariantMap& mapping)
{
    HttpRequest* request = new HttpRequest;

    QString postData = "nickName=" + mapping["nickName"].toString() + "&account=" + mapping["account"].toString()
        + "&password=" + mapping["password"].toString();

    request->sendRequest(UserSignupUrl, HttpRequest::POST, postData);

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
            emit jsonDoc.object().contains("msg") ? failed(jsonDoc.object().value("msg").toString())
                                                  : failed(QStringLiteral("NO_RESPONSE_MESSAGE"));
        }
    });
}

void LoginWithQQMail::redirect(QQmlApplicationEngine* engine, const QUrl& url)
{
    Q_UNUSED(engine)

    Q_ASSERT(QmlEngine != nullptr);
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreated, qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine->rootContext()->setContextProperty(QStringLiteral("UserModel"), mUser.data());
    engine->load(url);
}

void LoginWithQQMail::parse(const QVariantMap& userJson)
{
    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    mUser->mUID = userJson["id"].toString().toUInt();
    mUser->mNickName = userJson["nickName"].toString();
    mUser->mAccount = userJson["account"].toString();
    mUser->mPassword = userJson["password"].toString();
    mUser->mAvatar = userJson["avatar"].toString();
}
