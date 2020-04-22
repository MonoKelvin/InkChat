#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <Configuation.h>
#include <IChatObject.h>
#include <Utility.h>

#include <QCoreApplication>
#include <QSettings>

class AppSettings : public QSettings {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(AppSettings)

    explicit AppSettings(QObject* parent = nullptr);

public:
    ~AppSettings(void);

    // APP应用程序文件夹
    inline static const QString AppDir()
    {
        return QCoreApplication::applicationDirPath();
    }

    // APP数据文件夹
    inline static const QString AppDataDir()
    {
        return AppDir() + QStringLiteral("/Data/");
    }

    // APP配置文件
    inline static const QString AppConfigFile()
    {
        return AppDataDir() + QStringLiteral("Config.ini");
    }

    // 用户数据文件夹
    inline static const QString UserDataDir()
    {
        return UserDir() + QStringLiteral("/User");
    }

    // 用户数据文件
    inline static const QString UserDataFile()
    {
        return UserDataDir() + QStringLiteral("/User.udat");
    }

    // 聊天对象的缓存文件
    inline static const QString ChatObjectCacheFile(const QString& uuid)
    {
        return UserDataDir() + QStringLiteral("/Cache/") + uuid;
    }

    // 通过id来找到俩天对象的缓存文件
    static const QString ChatObjectCacheFile(unsigned int id);

    /**
     * @brief 用户聊天记录的数据库文件
     * @return 返回文件全称名，包含路径和后缀
     */
    static const QString MessageDBFile()
    {
        return UserDataDir() + QStringLiteral("/Message.db");
    }

    /**
     * @brief 用户头像地址
     * @param id 用户ID
     * @param size 要获取的大小
     * @return 返回头像http地址
     */
    inline static const QString UserAvatarUrl(unsigned int id, int size = 45)
    {
        return QString("http://inkchat.com/api/image.php?user=user&size=%1&id=%2").arg(size).arg(id);
    }

    // 用户缓存头像文件夹
    inline static const QString AvatarCacheFile(const QString& uuid)
    {
        return UserDir() + QStringLiteral("/Avatar/") + uuid;
    }

    // 聊天缓存图片文件夹
    inline static const QString ImageCacheDir()
    {
        return UserDir() + QStringLiteral("/Image/");
    }

    // 聊天缓存文件文件夹
    inline static const QString FileCacheDir()
    {
        return UserDir() + QStringLiteral("/File/");
    }

    // 聊天缓存视频文件夹
    inline static const QString VideoCacheDir()
    {
        return UserDir() + QStringLiteral("/Video/");
    }

    // 聊天缓存音频文件夹
    inline static const QString AudioCacheDir()
    {
        return UserDir() + QStringLiteral("/Audio/");
    }

    /**
     * @brief 获得当前用户id，当前用户是指最后一个登录系统的用户
     * @return 返回当前用户id
     */
    inline unsigned int getCurrentUserId(void) const
    {
        return Instance()->value(QStringLiteral("User/currentUser")).toUInt();
    }

    /**
     * @brief 设置当前用户id，当前用户是指最后一个登录系统的用户，一般只在自动登录情况下使用
     * @param id 当前用户id
     */
    inline void setCurrentUserId(unsigned int id)
    {
        Instance()->setValue(QStringLiteral("User/currentUser"), id);
    }

    inline static QVariant Value(const QString& key, const QVariant& defaultValue = QVariant()) noexcept
    {
        return Instance()->value(key, defaultValue);
    }

    inline static void SetValue(const QString& key, const QVariant& defaultValue = QVariant()) noexcept
    {
        Instance()->setValue(key, defaultValue);
    }

    inline static QSharedPointer<AppSettings> Instance()
    {
        static QSharedPointer<AppSettings> instance(new AppSettings);
        return instance;
    }

    /**
     * @brief 获取离线用户文件夹
     * @param userName 要获取的用户数据的用户名
     * @return 返回用户数据文件夹目录
     */
    inline static const QString UserDir(const QString& userName)
    {
        if (AppState == EAppState::Offline) {
            return QStringLiteral("/0/") + userName;
        }

        return QString();
    }

    /**
     * @brief UserDir 获取在线用户文件夹
     * @param onlineUserId 在线用户id索引
     * @return 返回用户数据文件夹目录 
     */
    inline static const QString UserDir(int onlineUserId)
    {
        if (AppState == EAppState::Online) {
            return AppDataDir() + QString::number(onlineUserId);
        }

        return QString();
    }

    inline static bool IsOffline(void)
    {
        return AppState == EAppState::Offline;
    }

    /**
     * @brief 自动判断当前用户状态下的用户数据目录（不推荐使用）
     * @return 返回用户数据文件夹目录 
     */
    static const QString UserDir();

    static void SetIconTheme(const QString& themeName);

Q_SIGNALS:
    void onAppThemeChanged();

    /** 离线标识，如果为true则系统处于离线状态 */
    static EAppState AppState;
};

#endif // APPSETTINGS_H
