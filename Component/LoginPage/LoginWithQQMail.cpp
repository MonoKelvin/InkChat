#include "LoginWithQQMail.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>

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

void LoginWithQQMail::loginRequest(const QVariantMap& mapping)
{
    static bool skip = AppSettings::Value("login/autoLogin", false).toBool();
    const auto user = User::Instance();
    auto account = mapping[QStringLiteral("account")].toString();
    auto password = mapping[QStringLiteral("password")].toString();

    HttpRequest* request = new HttpRequest;

    // 判断是否可以跳过登录
    if (skip && user->hasCache()) {
        // 当用户取消自动登录时，应该强制为不可跳过登录
        skip = false;

        account = user->mAccount;
        password = user->mPassword;
        emit autoLogin(account, password);

        // 封装请求数据
        const auto postData = QStringLiteral("id=%1&account=%2&password=%3").arg(user->mID).arg(account).arg(password);
        request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);
    } else {
        const auto postData = "account=" + mapping["account"].toString() + "&password=" + mapping["password"].toString();
        request->sendRequest(LoginByPasswordUrl, HttpRequest::POST, postData);
    }

    connect(request, &HttpRequest::request, [this, account, password](bool success, const QByteArray& jsonData) {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
        if (success) {
            if (err.error == QJsonParseError::NoError) {
                auto json = jsonDoc.object();

                json.insert(QLatin1String("account"), account);
                json.insert(QLatin1String("password"), password);

                try {
                    const auto user = User::Instance();
                    user->fromJson(json);
                    AppSettings::SetValue("user/currentUser", user->mID);
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
