#include "AppSettings.h"

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
    mCurrentUser = value(QStringLiteral("User/currentUser"), 0).toUInt();
}

AppSettings::~AppSettings()
{
}

void AppSettings::LoadAppTheme(const QString& themeFile)
{
    Q_UNUSED(themeFile)
}
