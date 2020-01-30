#include "LoginWithQQMail.h"

#include "../User.h"
#include "HttpRequest.h"
#include "../InkChatApi.h"
#include "../Widget/PromptWidget.h"

#include <QMap>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

QSharedPointer<User> ILoginOperation::mUser = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject *parent)
    : ILoginOperation(parent)
{

}

QSharedPointer<User> LoginWithQQMail::parse(const QString &jsonData)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8(), &jsonError);

    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject jsonObj = jsonDoc.object();

        auto user = QSharedPointer<User>(new User);

        user->UID = unsigned(jsonObj.value("id").toString().toInt());
        user->NickName = jsonObj.value("name").toString();
        user->Account = jsonObj.value("account").toString();
//        user->setPassword(jsonObj.value("password").toString());
//        user->setAvatarUrl(jsonObj.value("avatar").toString());
        user->MD5 = jsonObj.value("md5").toString();

        return user;
    }

    return nullptr;
}

void LoginWithQQMail::verify(const QMap<QString, QString> &mapping)
{
    HttpRequest *request = new HttpRequest;

    QString postData = "account=" + mapping["account"] + "&password=" + mapping["password"];
    request->sendRequest(UserLoginUrl, HttpRequest::POST, postData);

    connect(request, &HttpRequest::request, [ = ](bool success, const QByteArray & jsonData)
    {
        if (success)
        {
            mUser = parse(QString(jsonData));
            emit logedin();
        }
        else
        {
            QJsonParseError jsonError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
            if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject())
            {
                emit failed(jsonDoc.object().value("msg").toString());
            }
        }
    });
}

void LoginWithQQMail::signup(const QMap<QString, QString> &mapping)
{
    HttpRequest *request = new HttpRequest;
    QString postData = "name=" + mapping["name"]
                       + "&account=" + mapping["account"]
                       + "&password=" + mapping["password"];
    request->sendRequest(UserSignupUrl, HttpRequest::POST, postData);
    connect(request, &HttpRequest::request, [ = ](bool success, const QByteArray & jsonData)
    {
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
        if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (success)
            {
                emit registered();
            }
            else
            {
                emit failed(jsonObj.value("msg").toString());
            }
        }
        else
        {
            emit failed("注册信息不完整！");
        }
    });
}
