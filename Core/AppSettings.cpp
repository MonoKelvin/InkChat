#include "AppSettings.h"

AppSettings* AppSettings::mInstance = nullptr;
AppSettings::_GarbageCollection AppSettings::_GC;

AppSettings::AppSettings(const QString& fileName, QObject* parent)
    : QSettings(fileName, IniFormat, parent)
{

}
