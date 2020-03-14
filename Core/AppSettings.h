#ifndef APPSETTINGS_H
#define APPSETTINGS_H

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
        return UserDir() + QStringLiteral("/User/");
    }

    // 用户数据文件
    inline static const QString UserDataFile()
    {
        return UserDataDir() + QStringLiteral("User.udat");
    }

    /**
     * @brief 用户聊天记录文件
     * @param type 角色类型，即聊天对象类型
     * @return 返回文件全称名，包含路径和后缀
     */
    inline static const QString MessageCacheFile(IChatObject::ERoleType type = IChatObject::Friend);

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

    // 局域网数据文件夹
    inline static const QString LanDataDir()
    {
        return UserDir() + QStringLiteral("/LANs/");
    }

    // 用户缓存头像文件夹
    inline static const QString AvatarCacheFile(int roleType, unsigned int id)
    {
        return UserDir() + QStringLiteral("/Avatar/%1-%2.thumb").arg(roleType).arg(id);
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

    inline void setCurrentUserId(unsigned int id)
    {
        CurrentUserId = id;
        Instance()->setValue("User/currentUser", id);
    }

    inline unsigned int getCurrentUserId()
    {
        return CurrentUserId;
    }

    static void LoadAppTheme(const QString& themeFile);

    inline static QVariant Value(const QString& key, const QVariant& defaultValue = QVariant())
    {
        return Instance()->value(key, defaultValue);
    }

    inline static void SetValue(const QString& key, const QVariant& defaultValue = QVariant())
    {
        Instance()->setValue(key, defaultValue);
    }

    inline static QSharedPointer<AppSettings> Instance()
    {
        static QSharedPointer<AppSettings> instance(new AppSettings);
        return instance;
    }

    // 用户文件夹
    inline static const QString UserDir()
    {
        QString res = AppDataDir() + QString::number(Instance()->CurrentUserId);

        if (!OfflineUserName.isEmpty()) {
            res += QStringLiteral("/") + OfflineUserName;
        }

        return res;
    }

    /** 离线用户名，如果为空则默认是在线用户 */
    static QString OfflineUserName;

Q_SIGNALS:
    void onAppThemeChanged();

private:
    /** 当前用户id，离线用户永远是0 */
    unsigned int CurrentUserId;
};

#endif // APPSETTINGS_H
