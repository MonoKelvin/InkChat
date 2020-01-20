#include "LoginWithQQMail.h"

#include "Utility/HttpRequest.h"
#include "Utility/BookStudyAPI.h"
#include "Widget/PromptWidget.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

QSharedPointer<User> ILoginOperation::mUser = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject *parent)
    : ILoginOperation(parent)
{

}

UserModel *LoginWithQQMail::parse(const QString &jsonData)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8(), &jsonError);

    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject jsonObj = jsonDoc.object();
        UserModel *user = new UserModel;

        user->setID(unsigned(jsonObj.value("id").toString().toInt()));
        user->setName(jsonObj.value("name").toString());
        user->setAccount(jsonObj.value("account").toString());
//        user->setPassword(jsonObj.value("password").toString());
        user->setAvatarUrl(jsonObj.value("avatar").toString());
        user->setMD5(jsonObj.value("md5").toString());

        return user;
    }

    return nullptr;
}

void LoginWithQQMail::verify(const QMap<QString, QString> &mapping)
{
    HttpRequest *request = new HttpRequest;

    QString postData = "type=login&account=" + mapping["account"] + "&password=" + mapping["password"];
    request->sendRequest(UserLogInOut, HttpRequest::HttpRequestType::POST, postData);

    connect(request, &HttpRequest::request, [ = ](bool success, const QByteArray & jsonData)
    {
        if (success)
        {
            mUser = QSharedPointer<UserModel>(parse(QString(jsonData)));
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
    request->sendRequest(UserSignup, HttpRequest::HttpRequestType::POST, postData);
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
