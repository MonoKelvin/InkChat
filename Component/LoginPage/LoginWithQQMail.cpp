#include "LoginWithQQMail.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlContext>

QPointer<QQmlApplicationEngine> LoginWithQQMail::QmlEngine = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject* parent)
    : ILoginOperation(parent)
{
}

void LoginWithQQMail::loginRequest(const QVariantMap& mapping)
{
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

                try {
                    const auto user = User::Instance();
                    user->fromJson(json);
                    AppSettings::Instance()->setValue("user/currentUser", user->mID);
                    AppPaths::SetCurrentUserId(user->mID);
                    emit verified();
                } catch (const QString& msg) {
                    emit failed(msg);
                }
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

    qmlRegisterSingletonType<User>("UserModel", 1, 0, "UserModel", User::QmlSingletonTypeProvider);

    QmlEngine->load(url);
}

LoginWithQQMail::~LoginWithQQMail()
{
    if (!QmlEngine.isNull()) {
        QmlEngine.clear();
    }
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
    const auto skip = AppSettings::Instance()->value("login/autoLogin", false).toBool();
    if (!skip) {
        return false;
    }

    const auto user = User::Instance();

    if (!user->loadCache()) {
        emit failed(tr("自动登录验证失败，请尝试手动登录"));
        return false;
    }

    // 自动登录
    QVariantMap data;
    data.insert("account", user->mAccount);
    data.insert("password", user->mPassword);
    emit autoLogin(data);

    // Http请求
    HttpRequest* request = new HttpRequest;

    // 封装请求数据
    const auto postData = "id=" + QString::number(user->mID) + "&account=" + user->mAccount + "&password=" + user->mPassword;
    request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);

    // 验证是否自动登录成功
    connect(request, &HttpRequest::request, [user, this](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                auto json = jsonDoc.object();

                json.insert(QStringLiteral("account"), user->mAccount);
                json.insert(QStringLiteral("password"), user->mPassword);

                try {
                    user->fromJson(json);
                    AppSettings::Instance()->setValue("user/currentUser", user->mID);
                    AppPaths::SetCurrentUserId(user->mID);
                    emit verified();
                } catch (const QString& msg) {
                    emit failed(msg);
                }

            } else {
                emit failed(err.errorString());
            }
        } else {
            EMIT_FAILED_MESSAGE(jsonDoc, failed);
        }
    });

    return true;
}
