#include "LoginWithQQMail.h"

#include <User.h>
#include <InkChatApi.h>
#include <Http/HttpRequest.h>

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
        user->NickName = jsonObj.value("nickName").toString();
        user->Account = jsonObj.value("account").toString();
        user->Password = jsonObj.value("password").toString();
        user->Avatar = QPixmap(jsonObj.value("avatar").toString());
        user->MD5 = jsonObj.value("md5").toString();

        return user;
    }

    return nullptr;
}

void LoginWithQQMail::login(const QVariantMap &mapping)
{
    emit failed("ceshi"); //todo
    HttpRequest *request = new HttpRequest;

    // 解析登录信息
    QString postData = "account=" + mapping["account"].toString()
                       + "&password=" + mapping["password"].toString();

    request->sendRequest(UserLoginUrl, HttpRequest::POST, postData);
    qDebug() << postData;

    connect(request, &HttpRequest::request, [ = ](bool success, const QByteArray &jsonData)
    {
        if (success)
        {
            mUser = parse(QString(jsonData));
            emit verified();
        } else {
            QJsonParseError jsonError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
            if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject())
            {
                emit failed(jsonDoc.object().value("msg").toString());
            }
        }
    });
}

void LoginWithQQMail::signup(const QVariantMap &mapping)
{
    HttpRequest *request = new HttpRequest;

    // 解析注册信息
    QString postData = "nickName=" + mapping["nickName"].toString()
                       + "&account=" + mapping["account"].toString()
                       + "&password=" + mapping["password"].toString();

    request->sendRequest(UserSignupUrl, HttpRequest::POST, postData);
    qDebug() << postData;

    connect(request, &HttpRequest::request, [ = ](bool success, const QByteArray &jsonData)
    {
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
        if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (success) {
                emit registered();
            } else {
                emit failed(jsonObj.value("msg").toString());
            }
        } else {
            emit failed(tr("注册信息不完整！"));
        }
    });
}
