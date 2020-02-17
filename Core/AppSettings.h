#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <IChatObject.h>
#include <InkChatApi.h>
#include <Singleton.h>

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

    /**
     * @brief 用户头像地址
     * @param id 用户ID
     * @param size 要获取的大小
     * @return 返回头像http地址
     */
    inline static const QString UserAvatarUrl(unsigned int id, IChatObject::EAvatarSize size = IChatObject::AvatarSizeThumb)
    {
        return QString("http://inkchat.com/api/image.php?user=user&size=%1&id=%2").arg(size).arg(id);
    }

    // 用户聊天记录文件夹
    inline static const QString MessageCacheDir()
    {
        return UserDir() + QStringLiteral("/message/");
    }

    // 用户缓存头像文件夹
    inline static const QString AvatarCacheFile(int roleType, unsigned int id)
    {
        return UserDir() + QStringLiteral("/avatar/%1-%2.thumb").arg(roleType).arg(id);
    }

    // 聊天缓存图片文件夹
    inline static const QString ImageCacheDir()
    {
        return UserDir() + QStringLiteral("/image/");
    }

    // 聊天缓存文件文件夹
    inline static const QString FileCacheDir()
    {
        return UserDir() + QStringLiteral("/file/");
    }

    // 聊天缓存视频文件夹
    inline static const QString VideoCacheDir()
    {
        return UserDir() + QStringLiteral("/video/");
    }

    // 聊天缓存音频文件夹
    inline static const QString AudioCacheDir()
    {
        return UserDir() + QStringLiteral("/audio/");
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

class AppSettings : public QSettings, public Singleton<AppSettings> {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(AppSettings)

public:
    explicit AppSettings(QObject* parent = nullptr);
    ~AppSettings() = default;

    static void LoadAppTheme(const QString& themeFile);

    static inline QVariant Value(const char* key, const QVariant& defaultValue = QVariant())
    {
        return Instance()->value(QLatin1String(key), defaultValue);
    }

    static inline void SetValue(const char* key, const QVariant& defaultValue = QVariant())
    {
        Instance()->setValue(QLatin1String(key), defaultValue);
    }

Q_SIGNALS:
    void onAppThemeChanged();
};

#endif // APPSETTINGS_H
