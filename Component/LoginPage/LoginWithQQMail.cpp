#include "LoginWithQQMail.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QQmlApplicationEngine>
#include <QQmlContext>

QSharedPointer<User> ILoginOperation::mUser = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject *parent)
    : ILoginOperation(parent)
{
}

void LoginWithQQMail::loginRequest(const QVariantMap &mapping)
{
    HttpRequest *request = new HttpRequest;

    // 解析登录信息
    QString postData = "account=" + mapping["account"].toString()
                       + "&password=" + mapping["password"].toString();

    request->sendRequest(UserLoginUrl, HttpRequest::POST, postData);
    qDebug() << postData;

    connect(request,
            &HttpRequest::request,
            [=](bool success, const QByteArray &jsonData)
            {
                QJsonParseError err;
                QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
                if (success)
                {
                    if (err.error == QJsonParseError::NoError) {
                        parse(jsonDoc.object().toVariantMap());
                        emit verified();
                    } else {
                        emit failed(err.errorString());
                    }
                } else {
                    emit jsonDoc.object().contains("msg") ?
                    failed(jsonDoc.object().value("msg").toString()):
                    failed(QStringLiteral("NO_RESPONSE_MESSAGE"));
                }
            });
}

void LoginWithQQMail::signupRequest(const QVariantMap &mapping)
{
    HttpRequest *request = new HttpRequest;

    // 解析注册信息
    QString postData = "nickName=" + mapping["nickName"].toString()
                       + "&account=" + mapping["account"].toString()
                       + "&password=" + mapping["password"].toString();

    request->sendRequest(UserSignupUrl, HttpRequest::POST, postData);
    qDebug() << postData;

    connect(request,
            &HttpRequest::request,
            [=](bool success, const QByteArray &jsonData)
            {
                QJsonParseError err;
                if (success) {
                    if (err.error == QJsonParseError::NoError) {
                        emit registered();
                    } else {
                        emit failed(err.errorString());
                    }
                } else {
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &err);
                    emit jsonDoc.object().contains("msg") ?
                    failed(jsonDoc.object().value("msg").toString()):
                    failed(QStringLiteral("NO_RESPONSE_MESSAGE"));
                }
            });
}

void LoginWithQQMail::parse(const QVariantMap &userJson)
{
    mUser->UID = userJson["id"].toUInt();
    mUser->NickName = userJson["nickName"].toString();
    mUser->Account = userJson["account"].toString();
    mUser->Password = userJson["password"].toString();
    mUser->Avatar = QPixmap(userJson["avatar"].toString());
}

void LoginWithQQMail::redirect(QObject *ui)
{
    auto engine = qobject_cast<QQmlApplicationEngine *>(ui);
    if (engine) {
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        connect(
            engine,
            &QQmlApplicationEngine::objectCreated,
            qApp,
            [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl) {
                    QCoreApplication::exit(-1);
                }
            },
            Qt::QueuedConnection);

        // 加载页面
        engine->load(url);

        // 注入用户数据
        engine->rootContext()->setContextProperty("UserModel", mUser.data());
    }
}
