#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <InkChatApi.h>
#include <QSettings>

class AppSettings : public QSettings {
    Q_OBJECT
public:
    AppSettings(const QString& fileName, QObject* parent = nullptr);

    static AppSettings* Instance()
    {
        if (appSettings == nullptr) {
            appSettings = new AppSettings(APP_CONFIG_FILE);
        }

        return appSettings;
    }

    bool save();

Q_SIGNALS:
    void settingsChanged();

private:
    static AppSettings* appSettings;
};

#endif // APPSETTINGS_H
