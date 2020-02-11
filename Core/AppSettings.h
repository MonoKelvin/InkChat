#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <InkChatApi.h>
#include <QSettings>

class AppSettings : public QSettings {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(AppSettings)

    AppSettings(const QString& fileName, QObject* parent = nullptr);

public:
    static AppSettings* Instance(void)
    {
        if (nullptr == mInstance) {
            mInstance = new AppSettings(APP_CONFIG_FILE);
        }

        return mInstance;
    }

    static void LoadAppTheme(const QString& themeFile);

Q_SIGNALS:
    void onAppThemeChanged();

private:
    static AppSettings* mInstance;

    // 垃圾回收类
    class _GarbageCollection {
    public:
        _GarbageCollection() = default;
        ~_GarbageCollection()
        {
            if (mInstance != nullptr) {
                delete mInstance;
                mInstance = nullptr;
            }
        }
    };
    static _GarbageCollection _GC;
};

#endif // APPSETTINGS_H
