#include "LoginWithQQMail.h"

#include <Http/HttpRequest.h>
#include <InkChatApi.h>
#include <User.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QQmlContext>

QSharedPointer<User> ILoginOperation::mUser = nullptr;

LoginWithQQMail::LoginWithQQMail(QObject* parent)
    : ILoginOperation(parent)
{
    qmlRegisterType<LoginWithQQMail>("LoginWithQQMail", 1, 0, "LoginWithQQMail");

    const QUrl url(QStringLiteral("qrc:/LoginPage/LoginPage.qml"));
    QObject::connect(
        &Engine,
        &QQmlApplicationEngine::objectCreated,
        qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    Engine.rootContext()->setContextProperty("loginOperation", this);
    Engine.load(url);

    const auto qmlWin = Engine.rootObjects().first();
    connect(this, &LoginWithQQMail::failed, [qmlWin](const QString& msg) {
        QMetaObject::invokeMethod(qmlWin, "slotFailed", Q_ARG(QVariant, msg));
    });
    connect(this, &LoginWithQQMail::verified, [qmlWin, this] {
        QMetaObject::invokeMethod(qmlWin, "slotVerified");
        this->redirect(&(this->Engine));
    });
    connect(this, &LoginWithQQMail::registered, [qmlWin] {
        QMetaObject::invokeMethod(qmlWin, "slotRegistered");
    });
    connect(&Engine, &QQmlApplicationEngine::quit, this, &LoginWithQQMail::deleteLater);
}

LoginWithQQMail::~LoginWithQQMail()
{
    qDebug() << "deleted";
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
    qDebug() << postData;

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

void LoginWithQQMail::parse(const QVariantMap& userJson)
{
    if (mUser.isNull()) {
        mUser = QSharedPointer<User>(new User);
    }

    mUser->UID = userJson["id"].toString().toUInt();
    mUser->NickName = userJson["nickName"].toString();
    mUser->Account = userJson["account"].toString();
    mUser->Password = userJson["password"].toString();
    mUser->Avatar = QPixmap(userJson["avatar"].toString());
}

void LoginWithQQMail::redirect(QObject* ui)
{
    auto engine = qobject_cast<QQmlApplicationEngine*>(ui);
    if (engine) {
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        connect(
            engine, &QQmlApplicationEngine::objectCreated, qApp,
            [url](QObject* obj, const QUrl& objUrl) {
                if (!obj && url == objUrl) {
                    QCoreApplication::exit(-1);
                }
            },
            Qt::QueuedConnection);

        engine->rootContext()->setContextProperty("UserModel", mUser.data());

        // 加载页面
        engine->load(url);
    }
}
