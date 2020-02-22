#include "AppSettings.h"

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppConfigFile(), IniFormat, parent)
{
    mCurrentUser = value(QStringLiteral("user/currentUser"), 0).toUInt();
}

AppSettings::~AppSettings()
{
    qDebug() << "AppSettings Destroyed";
}

void AppSettings::LoadAppTheme(const QString& themeFile)
{
    Q_UNUSED(themeFile)
}
