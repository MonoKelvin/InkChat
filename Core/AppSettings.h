#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <IChatObject.h>
#include <InkChatApi.h>
#include <QSettings>

class AppPaths {
public:
    // APP应用程序文件夹
    inline static const QString AppDir()
    {
        return QCoreApplication::applicationDirPath();
    }

    // APP数据文件夹
    inline static const QString AppDataDir()
    {
        return AppDir() + QStringLiteral("/data/");
    }

    // APP配置文件
    inline static const QString AppConfigFile()
    {
        return AppDataDir() + QStringLiteral("config.ini");
    }

    // 用户数据文件夹
    inline static const QString UserDataDir()
    {
        return UserDir() + QStringLiteral("/user/");
    }

    // 用户数据文件
    inline static const QString UserDataFile() noexcept
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(QString::number(CurrentUser).toLatin1());
        auto result = QString(hash.result().toHex());
        return UserDataDir() + result + QStringLiteral(".udata");
    }

    // 用户头像
    inline static const QString UserAvatarUrl(IChatObject::EAvatarSize size)
    {
        return QStringLiteral("http://inkchat.com/api/image.php?user=user&size=%1&id=%2").arg(size).arg(CurrentUser);
    }

    // 群聊头像
    inline static const QString GroupAvatarUrl(IChatObject::EAvatarSize size)
    {
        return QStringLiteral("http://inkchat.com/api/image.php?user=group&size=%1&id=%2").arg(size).arg(CurrentUser);
    }

    // 用户聊天记录文件夹
    inline static const QString MessageCacheDir()
    {
        return UserDir() + QStringLiteral("/cache/message/");
    }

    // 用户缓存头像文件夹
    inline static const QString AvatarCacheDir(const QString& md5)
    {
        return UserDir() + QStringLiteral("/cache/avatar/") + md5;
    }

    // 聊天缓存图片文件夹
    inline static const QString ImageCacheDir()
    {
        return UserDir() + QStringLiteral("/cache/image/");
    }

    // 聊天缓存文件文件夹
    inline static const QString FileCacheDir()
    {
        return UserDir() + QStringLiteral("/cache/file/");
    }

    // 聊天缓存视频文件夹
    inline static const QString VideoCacheDir()
    {
        return UserDir() + QStringLiteral("/cache/video/");
    }

    // 聊天缓存音频文件夹
    inline static const QString AudioCacheDir()
    {
        return UserDir() + QStringLiteral("/cache/audio/");
    }

    inline static void SetCurrentUserId(unsigned int id)
    {
        CurrentUser = id;
    }

    inline static unsigned int GetCurrentUserId()
    {
        return CurrentUser;
    }

private:
    // 用户文件夹
    inline static const QString UserDir()
    {
        return AppPaths::AppDataDir() + QString::number(CurrentUser);
    }

    static unsigned int CurrentUser;
};

class AppSettings : public QSettings {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(AppSettings)

    AppSettings(const QString& fileName, QObject* parent = nullptr);

public:
    static AppSettings* Instance(void)
    {
        if (nullptr == mInstance) {
            mInstance = new AppSettings(AppPaths::AppConfigFile());
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
