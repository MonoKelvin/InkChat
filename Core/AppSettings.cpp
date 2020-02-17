#include "AppSettings.h"

unsigned int AppPaths::CurrentUser = 0;

AppSettings::AppSettings(QObject* parent)
    : QSettings(AppPaths::AppConfigFile(), IniFormat, parent)
{

}
