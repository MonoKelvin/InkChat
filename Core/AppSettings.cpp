#include "AppSettings.h"

#include <MessageDatabase.h>
#include <User.h>

#include <QIcon>

EAppState AppSettings::AppState = EAppState::Offline;

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
}

AppSettings::~AppSettings()
{
}

const QString AppSettings::ChatObjectCacheFile(unsigned int id)
{
}

const QString AppSettings::UserDir()
{
    if (AppState == EAppState::Online) {
        return AppDir() + QString::number(User::Instance()->getID());
    }

    return AppDir() + QStringLiteral("/0/") + User::Instance()->getNickName();
}

void AppSettings::SetIconTheme(const QString& themeName)
{
    const auto theme = QStringLiteral("App/theme");
    SetValue(theme, themeName);
    QIcon::setThemeName(themeName);
}
