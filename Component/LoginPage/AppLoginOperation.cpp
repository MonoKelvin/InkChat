#include "AppLoginOperation.h"

#include <AppSettings.h>
#include <FriendPage.h>
#include <LoginWithQQMail.h>
#include <MessageDatabase.h>
#include <MessagePage.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>

QPointer<QQmlApplicationEngine> AppLoginOperation::QmlEngine = nullptr;

AppLoginOperation::AppLoginOperation(QObject* parent)
    : ILoginOperation(parent)
{
}

AppLoginOperation::~AppLoginOperation()
{
    if (QmlEngine) {
        QmlEngine.clear();
    }
}

void AppLoginOperation::InitLoginPage(QQmlApplicationEngine* qmlEngine, const QUrl& url)
{
    QmlEngine = qmlEngine;

    // 注册到QML中
    qmlRegisterType<AppLoginOperation>("AppLoginOperation", 1, 0, "AppLoginOperation");
    qmlRegisterType<LoginWithQQMail>("LoginWithQQMail", 1, 0, "LoginWithQQMail");

    connect(
        QmlEngine, &QQmlApplicationEngine::objectCreated, qApp,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    QmlEngine->load(url);
}

void AppLoginOperation::loginRequest(const QVariantMap& mapping)
{
    const auto& user = User::Instance();
    const auto nickName = mapping.value(QStringLiteral("nickName")).toString();

    AppSettings::OfflineUserName = nickName;
    const auto fileName = AppSettings::AppDataDir() + QStringLiteral("0/%1/User/User.udat").arg(nickName);

    if (!isFileExists(fileName)) {
        emit failed(tr("用户不存在或密码错误"));
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        // TODO: 解密

        auto jsonDoc = QJsonDocument::fromJson(file.readAll());
        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            user->fromJson(jsonDoc.object());
            if (user->getPassword() == mapping.value(QStringLiteral("password"))
                && user->getNickName() == nickName) {
                AppSettings::Instance()->CurrentUserId = 0;
                emit verified();
            } else {
                emit failed(tr("密码错误"));
            }
        } else {
            emit failed(tr("用户数据解析出错"));
        }
    }

    file.close();
}

void AppLoginOperation::signupRequest(const QVariantMap& mapping)
{
    AppSettings::OfflineUserName = mapping.value(QStringLiteral("nickName")).toString();

    if (hasIllegalCharInFile(AppSettings::OfflineUserName)) {
        emit failed(tr("数据目录创建失败，用户名可能包含非法字符：\\/\"*?<>|"));
        return;
    }

    // 转换为json格式
    QJsonObject json;
    for (auto iter = mapping.cbegin(); iter != mapping.cend(); ++iter) {
        json.insert(iter.key(), iter.value().toJsonValue());
    }

    AppSettings::Instance()->CurrentUserId = 0;
    User::Instance()->fromJson(json);
    emit registered();
}

bool AppLoginOperation::isUserExists(const QString& userName)
{
    AppSettings::OfflineUserName = userName;

    const auto dirs = QDir(AppSettings::AppDataDir() + QStringLiteral("0/")).entryList(QDir::Dirs);

    for (int i = 0; i < dirs.size(); i++) {
        if (dirs.at(i) == userName) {
            return true;
        }
    }

    return false;
}

void AppLoginOperation::redirect(const QUrl& url, QQmlApplicationEngine* engine)
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
}
