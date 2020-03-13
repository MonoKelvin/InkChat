#include "AppSettings.h"

QString AppSettings::OfflineUserName;

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
    CurrentUserId = value(QStringLiteral("User/currentUser"), 0).toUInt();
}

AppSettings::~AppSettings()
{
}

void AppSettings::LoadAppTheme(const QString& themeFile)
{
    Q_UNUSED(themeFile)
}
