#include "AppLoginOperation.h"

#include <AppSettings.h>
#include <LoginWithQQMail.h>
#include <MessageDatabase.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>

AppLoginOperation::AppLoginOperation(QObject* parent)
    : ILoginOperation(parent)
{
}

AppLoginOperation::~AppLoginOperation()
{
}

void AppLoginOperation::loginRequest(const QVariantMap& mapping)
{
    const auto& user = User::Instance();
    const auto nickName = mapping.value(QStringLiteral("nickName")).toString();

    const auto fileName = AppSettings::AppDataDir() + QStringLiteral("/0/%1/User/User.udat").arg(nickName);

    if (!IsFileExists(fileName)) {
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
                user->setHostAddress(GetHostIP());
                emit verified();
            } else {
                emit failed(tr("用户名或密码错误"));
            }
        } else {
            emit failed(tr("用户数据解析出错"));
        }
    }

    file.close();
}

void AppLoginOperation::signupRequest(const QVariantMap& mapping)
{
    const QString name = mapping.value(QStringLiteral("nickName")).toString();
    const QString pwd = mapping.value(QStringLiteral("password")).toString();

    if (HasIllegalCharInFile(name)) {
        emit failed(tr("数据目录创建失败，用户名可能包含以下非法字符：\\/\"*?<>|"));
    } else if (name.trimmed().length() == 0) {
        emit failed(tr("用户名不可为空"));
    } else if (isUserExists(name)) {
        emit failed(tr("用户名已经存在"));
    } else if (pwd.length() < 6 || pwd.length() > 16) {
        emit failed(tr("请输入6-16位的密码"));
    } else {
        // 转换为json格式
        QJsonObject json;
        for (auto iter = mapping.cbegin(); iter != mapping.cend(); ++iter) {
            json.insert(iter.key(), iter.value().toJsonValue());
        }

        // 离线状态下生成必须使用uuid来标识用户
        json.insert(QLatin1String("uuid"), User::Instance()->generateUuid());

        User::Instance()->fromJson(json);
        emit registered();
    }
}

bool AppLoginOperation::isUserExists(const QString& userName)
{
    const auto dirs = QDir(AppSettings::AppDataDir() + QStringLiteral("/0/")).entryList(QDir::Dirs);

    for (int i = 0; i < dirs.size(); i++) {
        if (userName.compare(dirs.at(i), Qt::CaseInsensitive) == 0) {
            return true;
        }
    }

    return false;
}
