#include "AppSettings.h"

//#include <MessageDatabase.h>
#include <User.h>

#include <QIcon>

EAppState AppSettings::AppState = EAppState::Offline;

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
}

AppSettings::~AppSettings()
{
    qDebug() << "AppSettings Destroyed";
}

const QString AppSettings::UserDir() noexcept
{
    //if (AppState == EAppState::Online) {
    //    return AppDataDir() + QString::number(User::Instance()->getID());
    //}

    return AppDataDir() + QStringLiteral("/0/") + User::Instance()->getNickName();
}

void AppSettings::SetIconTheme(const QString& themeName)
{
    const auto theme = QStringLiteral("App/theme");
    SetValue(theme, themeName);
    QIcon::setThemeName(themeName);
}
