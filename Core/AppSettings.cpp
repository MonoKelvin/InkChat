#include "AppSettings.h"

SINGLETON_STATIC_INITIALIZA(AppSettings);

unsigned int AppPaths::CurrentUser = 0;

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppPaths::AppConfigFile(), IniFormat, parent)
{
}

AppSettings::~AppSettings()
{
    qDebug() << "ok";
}
