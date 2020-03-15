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

const QString AppSettings::MessageCacheFile(IChatObject::ERoleType type)
{
    QString res;

    switch (type) {
    case IChatObject::Friend:
        res = UserDataDir() + QStringLiteral("Message.db");
        break;
    case IChatObject::LAN:
        res = LanDataDir() + QStringLiteral("Message.db");
        break;
    default:
        break;
    }

    return res;
}

void AppSettings::LoadAppTheme(const QString& themeFile)
{
    Q_UNUSED(themeFile)
}
