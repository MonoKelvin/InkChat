#include "AppLoginOperation.h"

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
    if (user->hasCache() && user->getPassword() == mapping.value(QStringLiteral("password")).toString()) {
        emit verified();
    } else {
        emit failed(tr("密码错误"));
    }
}
