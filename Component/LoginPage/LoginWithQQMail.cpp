#include "LoginWithQQMail.h"

#include <AppSettings.h>
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
    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    HttpRequest* request = new HttpRequest;
    const auto postData = "account=" + mapping["account"].toString() + "&password=" + mapping["password"].toString();
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

                AppSettings::Instance()->setValue("user/currentUser", mUser->mID);

                emit verified();
            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc, failed);
        }
    });
}

void LoginWithQQMail::signupRequest(const QVariantMap& mapping)
{
    HttpRequest* request = new HttpRequest;

    const auto postData = "nickName=" + mapping["nickName"].toString() + "&account=" + mapping["account"].toString()
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

bool LoginWithQQMail::autoLoginRequest()
{
    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    const auto skip = AppSettings::Instance()->value("login/autoLogin", false).toBool();
    if (!skip) {
        return false;
    }

    if (!mUser->loadData()) {
        emit failed(tr("自动登录验证失败，请尝试手动登录"));
        return false;
    }

    // 自动登录
    QVariantMap data;
    data.insert("account", mUser->mAccount);
    data.insert("password", mUser->mPassword);
    emit autoLogin(data);

    // Http请求
    HttpRequest* request = new HttpRequest;

    // 封装请求数据
    const auto postData = "id=" + QString::number(mUser->mID) + "&account=" + mUser->mAccount + "&password=" + mUser->mPassword;
    request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);

    // 验证是否自动登录成功
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

    return true;
}
