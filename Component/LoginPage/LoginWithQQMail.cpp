#include "LoginWithQQMail.h"

#include <AppSettings.h>
#include <FriendPage.h>
#include <Http/HttpRequest.h>
#include <MessagePage.h>

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
//#include <QQmlContext>

QPointer<QQmlApplicationEngine> LoginWithQQMail::QmlEngine = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject* parent)
    : ILoginOperation(parent)
{
}

LoginWithQQMail::~LoginWithQQMail()
{
    if (!QmlEngine.isNull()) {
        QmlEngine.clear();
    }
}

void LoginWithQQMail::autoLoginRequest()
{
    static bool skip = AppSettings::Value(QStringLiteral("login/autoLogin"), false).toBool();
    const auto& user = User::Instance();

    // 判断是否可以跳过登录
    if (skip && user->hasCache()) {
        // 当取消自动登录时，应该强制为不可跳过登录
        skip = false;
        emit autoLogin(user->mAccount, user->mPassword);

        // 封装请求数据
        const auto postData = QStringLiteral("id=%1&account=%2&password=%3").arg(user->mID).arg(user->mAccount).arg(user->mPassword);

        HttpRequest* request = new HttpRequest;
        request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);

        connect(request, &HttpRequest::request, [this](bool success, const QByteArray& jsonData) {
            QJsonParseError err;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
            if (success) {
                if (err.error == QJsonParseError::NoError) {
                    auto json = jsonDoc.object();

                    const auto& user = User::Instance();

                    json.insert(QLatin1String("account"), user->mAccount);
                    json.insert(QLatin1String("password"), user->mPassword);

                    try {
                        user->fromJson(json);
                        AppSettings::Instance()->setCurrentUserId(user->mID);
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
}

void LoginWithQQMail::loginRequest(const QVariantMap& mapping)
{
    const auto account = mapping[QStringLiteral("account")].toString();
    const auto password = mapping[QStringLiteral("password")].toString();
    const auto postData = QStringLiteral("account=%1&password=%2").arg(account).arg(password);

    HttpRequest* request = new HttpRequest;
    request->sendRequest(LoginByPasswordUrl, HttpRequest::POST, postData);

    connect(request, &HttpRequest::request, [this, account, password](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                auto json = jsonDoc.object();

                json.insert(QLatin1String("account"), account);
                json.insert(QLatin1String("password"), password);

                try {
                    const auto& user = User::Instance();
                    user->fromJson(json);
                    AppSettings::Instance()->setCurrentUserId(user->mID);
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

    const auto postData = QStringLiteral("nickName=%1&account=%2&password=%3").arg(mapping["nickName"].toString()).arg(mapping["account"].toString()).arg(mapping["password"].toString());

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

    // 注册C++类型到QML
    qmlRegisterUncreatableType<IChatObject>("ChatObject", 1, 0, "ChatObject", "Cannot create ChatObject, because it is an interface");
    qmlRegisterUncreatableType<IChatItem>("ChatItem", 1, 0, "ChatItem", "Cannot create ChatItem, because it is an interface");
    qmlRegisterSingletonType<User>("UserModel", 1, 0, "UserModel", User::QmlSingletonTypeProvider);

    MESSAGEPAGE_INITIALIZA
    FRIENDPAGE_INITIALIZA

    connect(
        QmlEngine, &QQmlApplicationEngine::objectCreated, qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    QmlEngine->load(url);
}

void LoginWithQQMail::InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url)
{
    QmlEngine = qmlEngine;

    connect(
        QmlEngine, &QQmlApplicationEngine::objectCreated, qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    QmlEngine->load(url);
}
