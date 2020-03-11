#include "LoginWithQQMail.h"

#include <AppSettings.h>
#include <Http/HttpRequest.h>
#include <InkChatApi.h>

#include <QJsonDocument>
#include <QJsonObject>

LoginWithQQMail::LoginWithQQMail(QObject* parent)
    : ILoginOperation(parent)
{
}

LoginWithQQMail::~LoginWithQQMail()
{
}

void LoginWithQQMail::autoLoginRequest()
{
    bool skip = AppSettings::Value(QStringLiteral("Login/autoLogin"), false).toBool();
    const auto& user = User::Instance();

    // 判断是否可以跳过登录
    if (skip) {
        if (!user->hasCache()) {
            emit failed(QStringLiteral("用户消息读取出错"));
            return;
        }

        // 及时释放资源
        {
            auto data = QVariantMap();
            data.insert(QLatin1String("account"), user->getAccount());
            data.insert(QLatin1String("password"), user->getPassword());
            emit autoLogin(data);
        }

        // 封装请求数据
        const auto postData = QStringLiteral("id=%1&account=%2&password=%3").arg(user->getID()).arg(user->getAccount()).arg(user->getPassword());

        HttpRequest* request = new HttpRequest;
        request->sendRequest(LoginByIdUrl, HttpRequest::POST, postData);

        connect(request, &HttpRequest::request, [this](bool success, const QByteArray& jsonData) {
            QJsonParseError err;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
            if (success) {
                if (err.error == QJsonParseError::NoError) {
                    auto json = jsonDoc.object();

                    const auto& user = User::Instance();

                    json.insert(QLatin1String("account"), user->getAccount());
                    json.insert(QLatin1String("password"), user->getPassword());

                    try {
                        user->fromJson(json);
                        AppSettings::Instance()->setCurrentUserId(user->getID());
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
    } else {
        emit failed(QStringLiteral(""));
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
                    AppSettings::Instance()->setCurrentUserId(user->getID());
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
