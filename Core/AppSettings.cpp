#include "AppSettings.h"

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
    mCurrentUser = value(QStringLiteral("user/currentUser"), 0).toUInt();
}

void AppSettings::LoadAppTheme(const QString& themeFile)
{
    Q_UNUSED(themeFile)
}
