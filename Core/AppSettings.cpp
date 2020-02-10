#include "AppSettings.h"

AppSettings::AppSettings(const QString& fileName, QObject* parent)
    : QSettings(fileName, IniFormat, parent)
{

}
