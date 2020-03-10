#include "LoginDelegate.h"

#include <AppLoginOperation.h>
#include <FriendPage.h>
#include <LoginWithQQMail.h>
#include <MessagePage.h>

#include <QCoreApplication>

QPointer<QQmlApplicationEngine> LoginDelegate::QmlEngine = nullptr;

LoginDelegate::LoginDelegate(QObject *parent) : QObject(parent)
{
    auto sysLogin = new LoginWithQQMail(this);
    addLoginOperation(sysLogin, SystemLogin);

    auto qqmail = new LoginWithQQMail(this);
    addLoginOperation(qqmail, QQMailLogin);
}

LoginDelegate::~LoginDelegate()
{
    if (QmlEngine) {
        QmlEngine.clear();
    }
}

void LoginDelegate::InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url)
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

void LoginDelegate::addLoginOperation(ILoginOperation* loginOp, int loginId)
{
    loginOp->setParent(this);
    mLoginOperations.append(QPair<int, ILoginOperation*>(loginId, loginOp));
    connect(loginOp, &ILoginOperation::verified, [this] { emit verified(); });
    connect(loginOp, &ILoginOperation::registered, [this] { emit registered(); });
    connect(loginOp, &ILoginOperation::failed, [this](const QString& msg) { emit failed(msg); });
    connect(loginOp, &ILoginOperation::autoLogin, [this](const QVariantMap& mapping) { emit autoLogin(mapping); });
}

void LoginDelegate::redirectDelegate(QQmlApplicationEngine* engine, const QUrl& url, int loginId)
{
    Q_UNUSED(engine)
    Q_ASSERT(!QmlEngine.isNull());

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

    getLoginOperation(loginId)->redirect(engine, url);
}
