#include "AppSettings.h"

unsigned int AppPaths::CurrentUser = 0;

AppSettings* AppSettings::mInstance = nullptr;
AppSettings::_GarbageCollection AppSettings::_GC;

AppSettings::AppSettings(const QString& fileName, QObject* parent)
    : QSettings(fileName, IniFormat, parent)
{

}
